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

#include "GSSGetMic.h"
#include <cache/GSSContextCache.h>

#include <stdexcept>
#include <mit-krb5/gssapi/gssapi.h>

GSSGetMic::GSSGetMic ( JSONObject* params, gss_get_mic_type fn )
{
    this->function = fn;
    loadParameters(params);
}

/*
 *  {
 *       "context_handle": "#######",
 *       "qop_req": "GSS_C_QOP_DEFAULT",
 *       "input_message": "mary had a little lamb"
 *  }
 * 
 */
bool GSSGetMic::loadParameters ( JSONObject* params )
{
  std::string sQopReq, sConfReq, sInputMessage;
  
  /* Error checking */
  /* Setup */
  // Should I zeroOut?
  
  /* Main processing */
  
  /***********
   * QOP_REQ *
   ***********/
  if ( ! params->get("qop_req").isNull() )
  {
    if (params->get("qop_req").isString())
    {
      sQopReq = params->get("qop_req").string();
      if (sQopReq == "GSS_C_QOP_DEFAULT")
        this->qop_req = GSS_C_QOP_DEFAULT;
      else
        throw std::invalid_argument( std::string("Invalid QOP type given: ") + sQopReq );
    } else if (params->get("qop_req").isInteger())
      this->qop_req = (gss_cred_usage_t)( params->get("qop_req").integer() );
    else
      throw std::invalid_argument( "Unrecognized argument type for qop_req." );
  }
  
  /*****************
   * input_message *
   *****************/
  if ( ! params->get("input_message").isNull() )
  {
    sInputMessage = params->get("input_message").string();
    this->inputMessage.setValue(sInputMessage);
  }
  
  /******************
   * context_handle *
   ******************/
  if ( ! params->get("context_handle").isNull() )
  {
    sInputMessage = params->get("context_handle").string();
    GSSContext ctx = GSSContextCache::instance()->retrieve(sInputMessage);
    this->context = ctx.getContext();
  }
  
  /* Cleanup */
  /* Return */
  return true;
}

void GSSGetMic::execute()
{
    gss_buffer_desc output;
    
    this->retVal =  this->function(
      &(this->minor_status),
      this->context,
      this->qop_req,
      this->inputMessage.toGss(),
      &output
    );
    
    this->outputToken.setValue( (char *)(output.value), output.length );
}

JSONObject* GSSGetMic::toJSON()
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
    "output_token",
    this->outputToken.toString().c_str()
  );
  
  /* Cleanup */
  
  /* Return */
  return(values);
}
