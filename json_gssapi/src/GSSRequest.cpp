/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include <cstddef>
#include <stdexcept>

#include "commands/GSSAcquireCred.h"
#include "commands/GSSInitSecContext.h"
#include "commands/GSSImportName.h"
#include "GSSRequest.h"

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
  catch ( bad_alloc& e ) 
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
