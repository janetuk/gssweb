/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "GSSUnwrap.h"

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
  if ( ! params->get("arguments").get("input_message").isNull() )
  {
    sInputMessage = params->get("arguments").get("input_message").string();
    this->inputMessage.setValue(sInputMessage);
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
  JSONObject *ret = new JSONObject();
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
  
  // Put it all together.
  ret->set("command", "gss_wrap");
  ret->set("return_values", *values);
  
  /* Cleanup */
  
  /* Return */
  return(ret);
}
