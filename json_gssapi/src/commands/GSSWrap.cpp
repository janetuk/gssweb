/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "GSSWrap.h"
#include <cache/GSSContextCache.h>
#include <stdexcept>

/*
 *     gss_wrap_type function;
    int conf_req;
    gss_ctx_id_t context;
    gss_qop_t qop_req;
    GSSBuffer inputMessage;
    GSSBuffer outputMessage;
    
  
    OM_uint32 retVal;
    OM_uint32 minor_status;
    int conf_state;
 */

bool GSSWrap::loadParameters(JSONObject *params)
{
  /* Variables */
  std::string sQopReq, sConfReq, sInputMessage;
  
  /* Error checking */
  /* Setup */
  // Should I zeroOut?
  
  /* Main processing */
  
  /************
   * CONF_REQ *
   ************/
  if ( ! params->get("arguments").get("conf_req").isNull() )
  {
    if (params->get("arguments").get("conf_req").isString())
    {
      sConfReq = params->get("arguments").get("conf_req").string();
      if (sConfReq == "TRUE")
        this->conf_req = 1;
      else if (sConfReq == "FALSE")
        this->conf_req = 0;
      else
        throw std::invalid_argument( std::string("Invalid QOP type given: ") + sConfReq );
    } else if (params->get("arguments").get("conf_req").isInteger())
      this->conf_req = (gss_cred_usage_t)( params->get("arguments").get("conf_req").integer() );
    else
      throw std::invalid_argument( "Unrecognized argument type for conf_req." );
  }  
  
  /***********
   * QOP_REQ *
   ***********/
  if ( ! params->get("arguments").get("qop_req").isNull() )
  {
    if (params->get("arguments").get("qop_req").isString())
    {
      sQopReq = params->get("arguments").get("qop_req").string();
      if (sQopReq == "GSS_C_QOP_DEFAULT")
        this->qop_req = GSS_C_QOP_DEFAULT;
      else
        throw std::invalid_argument( std::string("Invalid QOP type given: ") + sQopReq );
    } else if (params->get("arguments").get("qop_req").isInteger())
      this->qop_req = (gss_cred_usage_t)( params->get("arguments").get("qop_req").integer() );
    else
      throw std::invalid_argument( "Unrecognized argument type for qop_req." );
  }
  
  /*****************
   * input_message *
   *****************/
  if ( ! params->get("arguments").get("input_message").isNull() )
  {
    sInputMessage = params->get("arguments").get("input_message").string();
    this->inputMessage.setValue(sInputMessage);
  }
  
  /***********
   * context *
   ***********/
  if ( ! params->get("arguments").get("context_handle").isNull() )
  {
    std::string contextKey = params->get("arguments").get("context_handle").string();
    GSSContext ctx = GSSContextCache::instance()->retrieve(contextKey);
    this->context = ctx.getContext();
  }
  
  /* Cleanup */
  /* Return */
  return true;
}

GSSWrap::GSSWrap ( JSONObject* params, gss_wrap_type fn )
{
  function = fn;
  loadParameters(params);
}


GSSWrap::~GSSWrap()
{

}

void GSSWrap::execute()
{
  /* Variables */
  gss_buffer_desc output_buf;
  
  retVal = function(
    &(this->minor_status),
    this->context,
    this->conf_req,
    this->qop_req,
    this->inputMessage.toGss(),
    &(this->conf_state),
    &output_buf
  );
  
  this->outputMessage.setValue( (char *)output_buf.value, output_buf.length );
}

/* Desired JSON output:
 * 
 * {
 *  "command":       "gss_wrap",
 *  "return_values": 
 *  {
 *      "major_status":   0,
 *      "minor_status":   0,
 *      "conf_state":     "TRUE",
 *      "output_message": "asdf"
 *  }
 * }
 */
JSONObject *GSSWrap::toJSON()
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

