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
  std::string input_name = (*params)["arguments"]["input_name"].string();
  std::string input_name_type = (*params)["arguments"]["input_name_type"].string();
  
  this->inputName.setValue( input_name );
  this->inputNameType.setValue( input_name_type );
  
  /* Cleanup */
  /* Return */
  return true;
}


/* Variables */
/* Error checking */
/* Setup */
/* Main */
/* Cleanup */
/* Return */
