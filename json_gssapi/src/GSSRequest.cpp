/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
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
//     response.set("error_message", e.what());
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
