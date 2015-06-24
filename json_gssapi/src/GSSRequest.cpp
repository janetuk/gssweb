/*
 * Copyright (c) 2014, 2015 JANET(UK)
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

#include <cstddef>
#include <stdexcept>
#include <string.h>

#include "commands/GSSAcquireCred.h"
#include "commands/GSSInitSecContext.h"
#include "commands/GSSImportName.h"
#include "commands/GSSDisplayName.h"
#include "GSSRequest.h"
#include "GSSException.h"

using std::bad_alloc;

GSSRequest::GSSRequest ( string jsonString )
{
  /* Local variables */
  /* Error checking */
  /* Setup */
  /* Main processing */
  response = JSONObject();
  cmd = NULL;
  requestString = jsonString;
  
  /* Cleanup */
  /* Return */
}

void GSSRequest::execute()
{
  try {
    /* variables */
    /* Error checking */
    /* Setup */
    parseJSON();
    getCommand();
    
    /* Main processing */
    if (NULL != cmd)
      cmd->execute();
    
    /* Cleanup */
    /* Return */
  }
  catch (GSSException e)
  {
    delete(cmd);
    cmd = NULL;
    JSONObject return_values;
    return_values.set("major_status", e.getMajor());
    return_values.set("minor_status", e.getMinor());
    return_values.set("what", e.what());
    response.set("return_values", return_values);
  }
}



void GSSRequest::parseJSON()
{
  /* variables */
  json_error_t jsonErr;
  
  try {
    JSONObject cookies;
    request = JSONObject::load(requestString.c_str(), 0, &jsonErr);
    cookies = request.get("cookies");
    response.set("cookies", cookies );
    response.set("method", request.get("method").string());
  }
  /* bad_alloc is thrown when JSONObject can't parse the input string as JSON */
  catch ( bad_alloc& ) 
  {
    // Top-level response
    response.set("error_message", "Could not parse the input JSON.");
    response.set("original_message", requestString.c_str());
  }
}


void GSSRequest::getCommand()
{
  string method;
  JSONObject arguments = request.get("arguments");
  
  /* Error checking */
  /* Setup */
  if (request.get("method").isNull() )
    method = "";
  else
    method = string( request.get("method").string() );
  
  if ( "gss_import_name" == method ) 
  {
    cmd = new GSSImportName ( &arguments );
  } 
  else if ( "gss_init_sec_context" == method ) 
  {
    cmd = new GSSInitSecContext ( &arguments );
  } 
  else if ( "gss_acquire_cred" == method ) 
  {
    cmd = new GSSAcquireCred ( &arguments );
  } 
  else if ( "gss_display_name" == method )
  {
    cmd = new GSSDisplayName ( &arguments );
  }
  else 
  {
    string error_message = string("Unrecognized command: ") + method;
    response.set("error_message", error_message.c_str() );
    response.set("original_message", requestString.c_str());
    cmd = NULL;
  }
}

string GSSRequest::getResponse()
{
  /* Variables */
  JSONObject *return_values;
  string gssResponse;
  
  /* Main processing */
  // Put the return values into the response, assuming that the command
  // was actually executed.
  if (NULL != cmd)
  {
    return_values = cmd->toJSON();
    response.set("return_values", *return_values);
  }
  
  // Convert the response into a string to return.
  gssResponse = string( response.dump() );
  
  /* Return */
  return(gssResponse);
}



char *gss_request(char *json_string)
{
  /* Variables */
  GSSRequest *req = new GSSRequest(string(json_string));
  
  /* Error checking */
  // An empty json_string could be an error, but GSSRequest does
  // a good job of handling it.
  
  /* Setup */
  /* Main processing */
  req->execute();
  return strdup(req->getResponse().c_str());
}

void deallocate_reply(char *reply)
{
  delete(reply);
}
