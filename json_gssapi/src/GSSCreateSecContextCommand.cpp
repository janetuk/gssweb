/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include "GSSCreateSecContextCommand.h"
#include "GSSException.h"
#include <gssapi.h>
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

/* Helper function - import the OID from a string */
static gss_OID str_to_oid(const char *mech_type_str = NULL)
{
  /* Variables */
  gss_buffer_desc gssbuffOID;
  gss_OID gssoidTargetOID;
  OM_uint32 major;
  OM_uint32 minor;
  
  /* Error checking */
  if (mech_type_str == NULL ||
      *mech_type_str == 0)
    return NULL;
  
  /* Setup */
  /* Main */
  gssbuffOID.value = (void *)(mech_type_str);
  gssbuffOID.length = strlen(mech_type_str);
  major = gss_str_to_oid(&minor, 
			 &gssbuffOID, 
			 &gssoidTargetOID);
  if (major != GSS_S_COMPLETE)
    throw GSSException("Error converting string to OID", major, minor);

  /* Cleanup */
  
  /* Return */
  return gssoidTargetOID;
}

void
GSSCreateSecContextCommand::execute()
{
  /* Variables */
  init_sec_context fn = (init_sec_context)function;
  
  /* Error checking */
  
  /* Setup */
  if (output_token.length > 0)
    retVal = gss_release_buffer(&minor_status, &output_token);
  
  /* Main */
  
  retVal = fn(
    &minor_status,
    GSS_C_NO_CREDENTIAL,
    &context_handle,
    target_name,
    mech_type,
    req_flags,
    time_req,
    GSS_C_NO_CHANNEL_BINDINGS,
    &input_token,
    &actual_mech_type,
    &output_token,
    &ret_flags,
    &time_rec);
  
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

const char* GSSCreateSecContextCommand::getActualMechType()
{
  return(this->oidToStr(this->actual_mech_type));
}

const char* GSSCreateSecContextCommand::getMechType()
{
  return(this->oidToStr(this->mech_type));
}

const char* GSSCreateSecContextCommand::oidToStr(gss_OID oid)
{
  gss_buffer_desc output;
  OM_uint32 major, minor;
  const char *retVal;

  /* error checking */
  if (oid == NULL)
    return NULL;

  /* Setup */  
  
  /* main */
  major = gss_oid_to_str(&minor, oid, &output);
  if (major == GSS_S_COMPLETE)
    retVal = (const char *)output.value;
  else
    retVal = NULL;
  
  /* cleanup */
  
  /* return */
  return( retVal );
}

bool GSSCreateSecContextCommand::loadParameters(JSONObject *params)
{
  /* Variables */
  OM_uint32 major, minor;
  gss_buffer_desc gssbuffTargetName;
  const char *buffer;
  
  /* Error checking */
  
  /* Setup */
  // Should I zeroOut?
  
  /* Main processing */
  // Easy stuff(*params)
  this->time_req = (OM_uint32)( (*params)["arguments"]["time_req"].integer() );
  this->req_flags = (OM_uint32)( (*params)["arguments"]["req_flags"].integer() );
  
  // context_handle
  //   -- just treat the value passed in as correct.
  context_handle = (gss_ctx_id_t)( (*params)["arguments"]["context_handle"].integer() );
  
  // target_name
  buffer = (*params)["arguments"]["target_name"].string();
  if (buffer != NULL && *buffer != 0)
  {
    gssbuffTargetName.value = (void *)buffer;
    gssbuffTargetName.length = strlen( buffer );
    
    major = gss_import_name(&minor, 
			    &gssbuffTargetName, 
			    GSS_C_NO_OID, 
			    &target_name);
    if (major != GSS_S_COMPLETE)
      throw GSSException("Error importing target_name", major, minor);
  }
  
  // mech_type  
  mech_type = str_to_oid( (*params)["arguments"]["mech_type"].string() );
  
  // input_token
  buffer = (*params)["arguments"]["input_token"].string();
  if (buffer != NULL && *buffer != 0)
  {
    this->input_token.value = (void *)buffer;
    this->input_token.length = strlen(buffer);
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
      
    if (mech_type != NULL)
      gss_release_oid(&minor, &(this->mech_type));
    
    if (this->actual_mech_type != NULL)
      gss_release_oid(&minor, &(this->actual_mech_type));
    
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
  mech_type = str_to_oid( "{ 1 2 840 113554 1 2 1 4 }" );
  this->input_token.length = 0;
  this->input_token.value = NULL;
  this->actual_mech_type = GSS_C_NO_OID;
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
  values->set("context_handle", (json_int_t)0);
  values->set("actual_mech_type", this->getActualMechType());
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
  void *fn) : GSSCommand(params, fn)
{
  zeroOut(false);
  loadParameters(params);
}

GSSCreateSecContextCommand::GSSCreateSecContextCommand(void *fn) : GSSCommand(fn)
{
  zeroOut(false);
}

