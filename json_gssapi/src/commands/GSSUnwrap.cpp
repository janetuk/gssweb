/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
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
