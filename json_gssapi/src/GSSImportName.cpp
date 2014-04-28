/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include "GSSImportName.h"

void GSSImportName::execute()
{

}

JSONObject *GSSImportName::toJSON()
{
  return new JSONObject();
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
  
  this->inputName.setValue( (*params)["arguments"]["input_name"].string() );
  this->inputNameType.setValue( (*params)["arguments"]["input_name_type"].string() );
  
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
