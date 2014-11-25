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

#include <stdexcept>

#include "GSSPseudoRandom.h"
#include <cache/GSSContextCache.h>
#include <datamodel/GSSContext.h>

GSSPseudoRandom::GSSPseudoRandom(JSONObject *params, 
					       gss_pseudo_random_type fn)
{
  /* Variables */
  /* Error checking */
  /* Setup */
  /* Main */
  loadParameters(params);
  function = fn;
  /* Cleanup */
  /* Return */
  
}

/* JSON input
 * 
 *   {
 *     "context_handle":     "########",
 *     "prf_key":            ###,
 *     "prf_in":             "la la la input message",
 *     "desired_output_len": ####
 *   }
 * 
 */
bool GSSPseudoRandom::loadParameters ( JSONObject* params )
{
  /* Variables */
  std::string sInputMessage;
  
  /* Error checking */
  /* Setup */
  /* Main */
  
  /***********
   * prf_key *
   ***********/
  if ( ! params->get("prf_key").isNull() )
  {
    if (params->get("prf_key").isInteger())
      this->key = (int )params->get("prf_key").integer();
    else
      throw std::invalid_argument( "Unrecognized argument type for prf_key." );
  }  

  
  /**********************
   * desired_output_len *
   **********************/
  if ( ! params->get("desired_output_len").isNull() )
  {
    if (params->get("desired_output_len").isInteger())
      this->desiredOutputLength = (int )params->get("desired_output_len").integer();
    else
      throw std::invalid_argument( "Unrecognized argument type for desired_output_len." );
  }  

  
  /**********
   * prf_in *
   **********/
  if ( ! params->get("prf_in").isNull() )
  {
    if (params->get("prf_in").isString())
    {
      sInputMessage = params->get("prf_in").string();
      this->inputMessage.setValue(sInputMessage);
    }
  }

  
  /******************
   * context_handle *
   ******************/
  if ( ! params->get("context_handle").isNull() )
  {
    if (params->get("context_handle").isString())
    {
      std::string contextKey = params->get("context_handle").string();
      GSSContext ctx = GSSContextCache::instance()->retrieve(contextKey);
      this->context = ctx.getContext();
    }
  }
  
  /* Cleanup */
  /* Return */
  return true;
}


void GSSPseudoRandom::execute()
{
  /* Variables */
  gss_buffer_desc output_buf;
  OM_uint32 minor = 0;
  
  retVal = function(
    &minor,
    this->context,
    this->key,
    this->inputMessage.toGss(),
    this->desiredOutputLength,
    &output_buf
  );
  
  this->minor_status = minor;
  this->outputMessage.setValue( (char *)output_buf.value, output_buf.length );
}

/* Desired JSON output:
 * 
 * {
 *  "command":       "gss_pseudo_random",
 *  "return_values": 
 *  {
 *      "major_status": 0,
 *      "minor_status": 0,
 *      "random_bytes": "asdf"
 *  }
 * }
 */
JSONObject* GSSPseudoRandom::toJSON()
{
  /* Variables */
  JSONObject *values = new JSONObject();
  
  /* Error checking */
  
  /* Setup */
  
  /* Main */
  // Return Values
  // Easy stuff
  values->set("major_status", this->retVal);
  values->set("minor_status", this->minor_status);
  
  values->set(
    "random_bytes",
    this->outputMessage.toString().c_str()
  );
  
  /* Cleanup */
  
  /* Return */
  return(values);
}

