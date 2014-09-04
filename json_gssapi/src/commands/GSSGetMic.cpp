/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
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
 * {"method": "gss_get_mic",
 *  "arguments":
 *  {
 *       "context_handle": "#######",
 *       "qop_req": "GSS_C_QOP_DEFAULT",
 *       "input_message": "mary had a little lamb"
 *  }
 * }
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
  
  /******************
   * context_handle *
   ******************/
  if ( ! params->get("arguments").get("context_handle").isNull() )
  {
    sInputMessage = params->get("arguments").get("context_handle").string();
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
    "output_token",
    this->outputToken.toString().c_str()
  );
  
  // Put it all together.
  ret->set("command", "gss_get_mic");
  ret->set("return_values", *values);
  
  /* Cleanup */
  
  /* Return */
  return(ret);
}
