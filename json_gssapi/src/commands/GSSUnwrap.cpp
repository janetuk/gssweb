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

#include "GSSUnwrap.h"
#include <cache/GSSContextCache.h>

GSSUnwrap::GSSUnwrap ( JSONObject* params, gss_unwrap_type fn )
{
  function = fn;
  loadParameters(params);
}

bool GSSUnwrap::loadParameters(JSONObject *params)
{
  /* Variables */
  std::string sInputMessage;
  
  /* Error checking */
  /* Setup */
  // Should I zeroOut?
  
  /* Main processing */
  
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


void GSSUnwrap::execute()
{
  /* Variables */
  gss_buffer_desc output_buf;
  
  retVal = function(
    &(this->minor_status),
    this->context,
    this->inputMessage.toGss(),
    &output_buf,
    &(this->conf_state),
    &(this->qop_state)
  );
  
  this->outputMessage.setValue( (char *)output_buf.value, output_buf.length );
}

JSONObject* GSSUnwrap::toJSON()
{
  /* Variables */
  const char *conf_state;
  JSONObject *values = new JSONObject();
  
  /* Error checking */
  
  /* Setup */
  
  /* Main */
  // Return Values
  // Easy stuff
  values->set("major_status", this->retVal);
  values->set("minor_status", this->minor_status);
  
  values->set("qop_state", this->qop_state);
  
  conf_state = (this->conf_state) ? "TRUE" : "FALSE";
  values->set("conf_state", conf_state);
  
  values->set(
    "output_message",
    this->outputMessage.toString().c_str()
  );
  
  /* Cleanup */
  
  /* Return */
  return(values);
}
