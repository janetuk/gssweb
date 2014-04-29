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
