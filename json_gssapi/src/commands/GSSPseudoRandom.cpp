/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include <stdexcept>

#include "GSSPseudoRandom.h"

GSSPseudoRandom::GSSPseudoRandom(JSONObject *params, 
					       gss_pseudo_random_type fn) : GSSCommand(params)
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
 * {"method":    "gss_pseudo_random",
 *  "arguments": 
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
  if ( ! params->get("arguments").get("prf_key").isNull() )
  {
    if (params->get("arguments").get("prf_key").isInteger())
      this->key = params->get("arguments").get("prf_key").integer();
    else
      throw std::invalid_argument( "Unrecognized argument type for prf_key." );
  }  

  
  /**********************
   * desired_output_len *
   **********************/
  if ( ! params->get("arguments").get("desired_output_len").isNull() )
  {
    if (params->get("arguments").get("desired_output_len").isInteger())
      this->desiredOutputLength = params->get("arguments").get("desired_output_len").integer();
    else
      throw std::invalid_argument( "Unrecognized argument type for desired_output_len." );
  }  

  
  /**********
   * prf_in *
   **********/
  if ( ! params->get("arguments").get("prf_in").isNull() )
  {
    if (params->get("arguments").get("prf_in").isString())
    {
      sInputMessage = params->get("arguments").get("prf_in").string();
      this->inputMessage.setValue(sInputMessage);
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
  JSONObject *ret = new JSONObject();
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
  
  // Put it all together.
  ret->set("command", "gss_pseudo_random");
  ret->set("return_values", *values);
  
  /* Cleanup */
  
  /* Return */
  return(ret);
}

