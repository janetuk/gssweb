/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include "GSSCreateSecContextCommand.h"
#include "GSSException.h"
#include <cache/GSSContextCache.h>
#include <cache/GSSNameCache.h>
#include <datamodel/GSSName.h>
#include <gssapi.h>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>

typedef OM_uint32 (*init_sec_context)(
    OM_uint32 *,        /* minor_status */
    gss_cred_id_t,      /* claimant_cred_handle */
    gss_ctx_id_t *,     /* context_handle */
    gss_name_t,         /* target_name */
    gss_OID,            /* mech_type (used to be const) */
    OM_uint32,          /* req_flags */
    OM_uint32,          /* time_req */
    gss_channel_bindings_t,     /* input_chan_bindings */
    gss_buffer_t,       /* input_token */
    gss_OID *,          /* actual_mech_type */
    gss_buffer_t,       /* output_token */
    OM_uint32 *,        /* ret_flags */
    OM_uint32 *         /* time_req */
);

void
GSSCreateSecContextCommand::execute()
{
  /* Variables */
  init_sec_context fn = (init_sec_context)function;
  gss_OID actual_mech_type;
  
  /* Error checking */
  
  /* Setup */
  if (output_token.length > 0)
    retVal = gss_release_buffer(&minor_status, &output_token);

  /* Main */
  retVal = fn(
    &minor_status,
    GSS_C_NO_CREDENTIAL,
    &context_handle,
    targetName.toGss(),
    mechType.toGss(),
    req_flags,
    time_req,
    GSS_C_NO_CHANNEL_BINDINGS,
    &input_token,
    &actual_mech_type,
    &output_token,
    &ret_flags,
    &time_rec);
  
  if ( GSS_ERROR(this->retVal) )
  {
    std::string errMsg;
    errMsg += "Cannot init_sec_context: ";
    throw GSSException(errMsg.c_str(), this->retVal, this->minor_status, mechType.toGss());
  }
  
  actualMechType.setValue(actual_mech_type);
  
  context.setContext(context_handle, true);
  contextKey = GSSContextCache::instance()->store(context);
  
  /* Cleanup */
  
  /* Return */
}

const char* GSSCreateSecContextCommand::getTargetDisplayName()
{
  /* Variables */
  gss_buffer_desc output_name;
  gss_OID output_type;
  OM_uint32 major, minor;
  const char *ret;
  
  /* error checking */
  
  /* Setup */
  
  /* Main */
  major = gss_display_name(&minor, target_name, &output_name, &output_type);
  if (major == GSS_S_COMPLETE)
    ret = (const char *)output_name.value;
  else
    ret = NULL;
  
  /* cleanup */
  
  /* return */
  return( ret );
}

bool GSSCreateSecContextCommand::loadParameters(JSONObject *params)
{
  /* Variables */
  std::string key;
  
  /* Error checking */
  if ( params->get("arguments").isNull() )
    return true;
  
  /* Setup */
  // Should I zeroOut?
  
  /* Main processing */
  // Easy stuff(*params)
  if ( !params->get("arguments").get("time_req").isNull() )
    this->time_req = params->get("arguments").get("time_req").integer();
  
  if ( !params->get("arguments").get("req_flags").isNull() )
    this->req_flags = params->get("arguments").get("req_flags").integer();
  
  // context_handle
  if ( ! params->get("arguments").get("context_handle").isNull() )
  {
    this->context_handle = GSS_C_NO_CONTEXT;
    if (params->get("arguments").get("context_handle").isString())
    {
      key = params->get("arguments").get("context_handle").string();
      context = GSSContextCache::instance()->retrieve( key.c_str() );
      this->context_handle = context.getContext();
    }
    if (GSS_C_NO_CONTEXT == this->context_handle)
      throw std::invalid_argument( "Could not find the context_handle." );
  }
  
  // target_name
  if ( ! params->get("arguments").get("target_name").isNull() )
  {
    this->target_name = GSS_C_NO_NAME;
    if (params->get("arguments").get("target_name").isString())
    {
      key = params->get("arguments").get("target_name").string();
      
      targetName = GSSNameCache::instance()->retrieve(key);
      
      this->target_name = targetName.toGss();
    }
    if (GSS_C_NO_NAME == this->target_name)
      throw std::invalid_argument( "Could not find the target_name" );
  }
  
  // mech_type  
  if ( ! params->get("arguments").get("mech_type").isNull() )
  {
    key.clear();
    if (params->get("arguments").get("mech_type").isString())
    {
      key = params->get("arguments").get("mech_type").string();
      mechType.setValue(key);
    }
    if (GSS_C_NO_OID == this->mechType.toGss() )
      throw std::invalid_argument( std::string() + "Could not create a mech_type OID from '" + key + "'");
  }
  
  // input_token
  if ( ! params->get("arguments").get("input_token").isNull() )
  {
    key = params->get("arguments").get("input_token").string();
    this->input_token.value = (void *)key.c_str();
    this->input_token.length = key.length();
  }

  /* Cleanup */
  
  
  /* Return */
  return true;
}

bool GSSCreateSecContextCommand::zeroOut(bool initialized)
{
  /* Error checking */
  /* Variables */
  OM_uint32 minor;
  gss_buffer_desc output;
  
  /* Setup */
  /* Main */

  // Free up existing memory if it's been set.  
  if (initialized)
  {
    if (this->context_handle != NULL)
      gss_delete_sec_context(&minor, &(this->context_handle), &output);
    
    if (this->target_name != NULL)
      gss_release_name(&minor, &(this->target_name));
      
    if (this->output_token.length > 0)
      gss_release_buffer(&minor, &output_token);
    
    if (this->input_token.length > 0)
      gss_release_buffer(&minor, &input_token);
  }

  // Now set things to reasonable defaults
  this->retVal = 0;
  this->minor_status = 0;
  this->req_flags = 0;
  this->time_req = 0;
  this->ret_flags = 0;
  this->time_rec = 0;

  this->context_handle = GSS_C_NO_CONTEXT;
  this->target_name = GSS_C_NO_NAME;
  this->mechType.setValue( (char *)"{ 1 3 6 1 5 5 15 1 1 18 }" );
  this->input_token.length = 0;
  this->input_token.value = NULL;
  this->output_token.length = 0;
  this->output_token.value = NULL;

  /* Cleanup */
  /* Return */
  return(true);
}

JSONObject *GSSCreateSecContextCommand::toJSON()
{
  /* Variables */
  JSONObject *ret = new JSONObject();
  JSONObject *values = new JSONObject();
  
  /* Error checking */
  
  /* Setup */
  
  /* Main */
  values->set("major_status", this->retVal);
  values->set("minor_status", this->minor_status);
  values->set("context_handle", this->contextKey.c_str());
  values->set("actual_mech_type", this->getActualMechType().toString().c_str());
  values->set("output_token", (const char *)this->output_token.value);
  values->set("ret_flags", this->ret_flags);
  values->set("time_rec", this->time_rec);
  ret->set("command", "gss_init_sec_context");
  ret->set("return_values", *values);
  
  /* Cleanup */
  
  /* Return */
  return(ret);
}

GSSCreateSecContextCommand::GSSCreateSecContextCommand(
  JSONObject *params, 
  void *fn) : GSSCommand(params)
{
  zeroOut(false);
  loadParameters(params);
  function = fn;
}

GSSCreateSecContextCommand::GSSCreateSecContextCommand(void *fn)
{
  zeroOut(false);
  function = fn;
}

