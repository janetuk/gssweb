/*
 * Copyright (c) 2014, JANET(UK)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of JANET(UK) nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
// MRW -- Add proper copyright boilerplate to all files

#include "GSSInitSecContext.h"
#include "GSSException.h"
#include <cache/GSSContextCache.h>
#include <cache/GSSNameCache.h>
#include <datamodel/GSSName.h>
#include <gssapi.h>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>

#include "utils/base64.h"

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
GSSInitSecContext::execute()
{
  /* Variables */
  init_sec_context fn = (init_sec_context)function;
  gss_OID actual_mech_type;
  
  /* Error checking */
  
  /* Setup */
  if (output_token.length > 0)
    retVal = gss_release_buffer(&minor_status, &output_token);

  /* Main */
  // MRW -- fix so that this uses all of the vars from the object 
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
    // MRW -- steal code from import name
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

const char* GSSInitSecContext::getTargetDisplayName()
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

bool GSSInitSecContext::loadParameters(JSONObject *params)
{
  /* Variables */
  std::string key;
  std::string token;
  size_t len;
  
  /* Error checking */
  if ( params->isNull() )
    return true;
  
  /* Setup */
  
  /* Main processing */
  // MRW -- finish parsing all of the variables
  // claimant_cred_handle

  // context_handle
  if ( ! params->get("context_handle").isNull() )
  {
    this->context_handle = GSS_C_NO_CONTEXT;
    if (params->get("context_handle").isString())
    {
      key = params->get("context_handle").string();
      context = GSSContextCache::instance()->retrieve( key.c_str() );
      this->context_handle = context.getContext();
    }
    if (GSS_C_NO_CONTEXT == this->context_handle)
      throw std::invalid_argument( "Could not find the context_handle." );
  }
  
  // target_name
  if ( ! params->get("target_name").isNull() )
  {
    this->target_name = GSS_C_NO_NAME;
    if (params->get("target_name").isString())
    {
      key = params->get("target_name").string();
      
      targetName = GSSNameCache::instance()->retrieve(key);
      
      this->target_name = targetName.toGss();
    }
    if (GSS_C_NO_NAME == this->target_name)
      throw std::invalid_argument( "Could not find the target_name" );
  }
  
  // mech_type  
  if ( ! params->get("mech_type").isNull() )
  {
    key.clear();
    if (params->get("mech_type").isString())
    {
      key = params->get("mech_type").string();
      mechType.setValue(key);
    }
    if (GSS_C_NO_OID == this->mechType.toGss() )
      throw std::invalid_argument( std::string() + "Could not create a mech_type OID from '" + key + "'");
  }
  
  // req_flags
  if ( !params->get("req_flags").isNull() )
    this->req_flags = params->get("req_flags").integer();

  // time_req
  if ( !params->get("time_req").isNull() )
    this->time_req = params->get("time_req").integer();

  // input_chennel_bindings

  // input_token
  if ( ! params->get("input_token").isNull() )
  {
    token = params->get("input_token").string();
    token = (char *)base64_decode(token, &len);
    this->input_token.value = (void *)token.c_str();
    this->input_token.length = token.length();
  }

  /* Cleanup */
  
  /* Return */
  return true;
}

bool GSSInitSecContext::zeroOut(bool initialized)
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

JSONObject *GSSInitSecContext::toJSON()
{
  /* Variables */
  // MRW -- values should be scoped to the class, so execute can set error values?
  std::string output_str;
  JSONObject *values = new JSONObject();
  
  /* Error checking */
  
  /* Setup */
  
  /* Main */
  values->set("major_status", this->retVal);
  values->set("minor_status", this->minor_status);
  values->set("context_handle", this->contextKey.c_str());
  values->set("actual_mech_type", this->getActualMechType().toString().c_str());
  // MRW -- is output_token.value guaranteed to be null-terminated?
  output_str = (char *)output_token.value;
  values->set("output_token", base64_encode(output_str));
  values->set("ret_flags", this->ret_flags);
  values->set("time_rec", this->time_rec);
  // MRW -- modify for new error handling

  /* Cleanup */
  
  /* Return */
  return(values);
}

GSSInitSecContext::GSSInitSecContext(
  JSONObject *params, 
  void *fn) : GSSCommand(params)
{
  zeroOut(false);
  loadParameters(params);
  function = fn;
}

GSSInitSecContext::GSSInitSecContext(void *fn)
{
  zeroOut(false);
  function = fn;
}

