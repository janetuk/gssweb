/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include "GSSImportName.h"
#include "GSSException.h"

void GSSImportName::execute()
{
  this->outputName = GSSName(inputName, inputNameType, function);
  this->retVal = this->outputName.getMajorStatus();
  this->minor_status = this->outputName.getMinorStatus();
  this->outputName.setKey("constant for now");
}

JSONObject *GSSImportName::toJSON()
{
  /* Variables */
  JSONObject *ret = new JSONObject();
  JSONObject *values = new JSONObject();
  
  /* Error checking */
  
  /* Setup */
  
  /* Main */
  values->set("major_status", this->retVal);
  values->set("minor_status", this->minor_status);
  values->set("gss_name", this->outputName.getKey().c_str() );
  ret->set("command", "gss_import_name");
  ret->set("return_values", *values);
  
  /* Cleanup */
  
  /* Return */
  return(ret);
}

GSSImportName::GSSImportName(JSONObject *params, gss_imp_name_type fn) : GSSCommand(params)
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

bool GSSImportName::loadParameters(JSONObject *params)
{
  /* Variables */
  
  /* Error checking */
  /* Setup */
  // Should I zeroOut?
  
  /* Main processing */
  // Easy stuff(*params)
  if ( params->get("arguments").isNull() )
    return true;
  
  if ( !params->get("arguments").get("input_name").isNull() )
  {
    if ( params->get("arguments").get("input_name").isString() )
    {
      std::string input_name = params->get("arguments").get("input_name").string();
      this->inputName.setValue( input_name );
    }
  }
  
  if ( !params->get("arguments").get("input_name_type").isNull() )
  {
    if ( params->get("arguments").get("input_name_type").isString() )
    {
      std::string input_name_type = params->get("arguments").get("input_name_type").string();
      this->inputNameType.setValue( input_name_type );
    }
  }
  
  /* Cleanup */
  /* Return */
  return true;
}


//   /***********
//    * QOP_REQ *
//    ***********/
//   if ( ! params->get("arguments").get("qop_req").isNull() )
//   {
//     if (params->get("arguments").get("qop_req").isString())
//     {
//       sQopReq = params->get("arguments").get("qop_req").string();
//       if (sQopReq == "GSS_C_QOP_DEFAULT")
//         this->qop_req = GSS_C_QOP_DEFAULT;
//       else
//         throw std::invalid_argument( std::string("Invalid QOP type given: ") + sQopReq );
//     } else if (params->get("arguments").get("qop_req").isInteger())
//       this->qop_req = (gss_cred_usage_t)( params->get("arguments").get("qop_req").integer() );
//     else
//       throw std::invalid_argument( "Unrecognized argument type for qop_req." );
//   }
//   
//   /*****************
//    * input_message *
//    *****************/
//   if ( ! params->get("arguments").get("input_message").isNull() )
//   {
//     sInputMessage = params->get("arguments").get("input_message").string();
//     this->inputMessage.setValue(sInputMessage);
//   }


/* Variables */
/* Error checking */
/* Setup */
/* Main */
/* Cleanup */
/* Return */
