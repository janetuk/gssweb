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

#include "GSSInitSecContext.h"
#include "GSSException.h"
#include <cache/GSSContextCache.h>
#include <cache/GSSCredentialCache.h>
#include <cache/GSSNameCache.h>
#include <datamodel/GSSDisplayStatus.h>
#include <datamodel/GSSName.h>
#include <gssapi.h>
#include <stdexcept>
#include <stdlib.h>
#include <string.h>

#include "utils/util_base64.h"

void
GSSInitSecContext::execute()
{
  /* Variables */
  gss_OID actual_mech_type;
  JSONObject errors;
  
  /* Error checking */
  
  /* Setup */
  if (output_token.length > 0)
    retVal = gss_release_buffer(&minor_status, &output_token);

  /* Main */
  retVal = function(
    &minor_status,
    claimantCred.toGss(),
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

  actualMechType.setValue(actual_mech_type);
  context.setContext(context_handle, true);
  contextKey = GSSContextCache::instance()->store(context);
  
  /* Cleanup */
  
  // Handle errors
  GSSDisplayStatus ds(retVal, minor_status, mechType.toGss());
  errors.set("major_status_message", ds.getMajorMessage().c_str());
  errors.set("minor_status_message", ds.getMinorMessage().c_str());
  values->set("errors", errors);


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
  
  /* Error checking */
  if ( params->isNull() )
    return true;
  
  /* Setup */
  
  /* Main processing */
  // claimant_cred_handle
  if (!(params->get("claimant_cred_handle").isNull() ||
	(params->get("claimant_cred_handle").isString() &&
	 std::string("") == params->get("claimant_cred_handle").string())))
  {
    std::string key = params->get("claimant_cred_handle").string();
    this->claimantCred = GSSCredentialCache::instance()->retrieve(key);
  }

  // context_handle
  if (!(params->get("context_handle").isNull() ||
	(params->get("context_handle").isString() &&
	 std::string("") == params->get("context_handle").string())))
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
  if (! (  params->get("target_name").isNull() ||
	   (params->get("target_name").isString() &&
	    std::string("") == params->get("target_name").string())
	)
     )
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
  if (! ( params->get("mech_type").isNull() ||
	  (
	    params->get("mech_type").isString() &&
	    std::string("") == params->get("mech_type").string()
	  )
	)
     )
  {
    key.clear();
    if (params->get("mech_type").isString())
    {
      key = params->get("mech_type").string();
      mechType.setValue(key);
    }
    if (GSS_C_NO_OID == this->mechType.toGss() )
      throw std::invalid_argument( std::string() + "Could not create a mech_type OID from '" + key + "'");
    if ( !(this->mechType.isGssEapMech()) )
      throw std::invalid_argument( std::string() +
				   "'" + key + "' must be 1.3.6.1.5.5.15.1.1.*");
  }
  
  // req_flags
  if (!params->get("req_flags").isNull() )
    this->req_flags = (OM_uint32 )params->get("req_flags").integer();

  // time_req
  if (!params->get("time_req").isNull() )
    this->time_req = (OM_uint32 )params->get("time_req").integer();

  // input_token
  if (! (params->get("input_token").isNull() ||
	 (params->get("input_token").isString() &&
	  std::string("") == params->get("input_token").string())))
  {
    token = params->get("input_token").string();
    input_token.value = base64Decode(token.c_str(), &input_token.length);
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
    
    if (this->input_token.value) {
      base64Free(input_token.value);
      input_token.value = NULL;
      input_token.length = 0;
    }
  }

  // Now set things to reasonable defaults
  this->retVal = 0;
  this->minor_status = 0;
  this->req_flags = 0;
  this->time_req = 0;
  this->ret_flags = 0;
  this->time_rec = 0;

  this->claimantCred = GSS_C_NO_CREDENTIAL;
  this->context_handle = GSS_C_NO_CONTEXT;
  this->target_name = GSS_C_NO_NAME;
  this->mechType.setValue( (char *)"{ 1 3 6 1 5 5 15 1 1 17 }" );
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
  std::string output_str;
  
  /* Error checking */
  
  /* Setup */
  base64EncodeStr(output_token.value, output_token.length, output_str);
  
  /* Main */
  values->set("major_status", this->retVal);
  values->set("minor_status", this->minor_status);

  if ( !GSS_ERROR(this->retVal) )
  {
    values->set("context_handle", this->contextKey.c_str());
    values->set("actual_mech_type", this->getActualMechType().toString().c_str());
    values->set("output_token", output_str.c_str());
    values->set("ret_flags", this->ret_flags);
    values->set("time_rec", this->time_rec);
  }

  /* Cleanup */
  
  /* Return */
  return(values);
}

GSSInitSecContext::GSSInitSecContext(
  JSONObject *params, 
  init_sec_context_type fn)
{
  zeroOut(false);
  loadParameters(params);
  function = fn;

  values = new JSONObject();
}

GSSInitSecContext::GSSInitSecContext(init_sec_context_type fn)
{
  zeroOut(false);
  function = fn;
}

