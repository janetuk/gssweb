/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include "GSSImportName.h"
#include "GSSException.h"
#include <cache/GSSNameCache.h>


typedef OM_uint32 (*gss_imp_name_type)(
    OM_uint32 *,        /* minor_status */
    gss_buffer_t,       /* input_name_buffer */
    gss_OID,            /* input_name_type(used to be const) */
    gss_name_t *);      /* output_name */



void GSSImportName::execute()
{
  /* Variables */
  gss_name_t name;
  std::string key;

  /* Error checking */
  /* Setup */

  /* Main */
  retVal = function(&minor_status, inputName.toGss(), inputNameType.toGss(), &name);
  if ( GSS_ERROR(this->retVal) )
  {
    std::string errMsg;
    errMsg += "Cannot import name: ";
    errMsg += inputName.toString();
    throw GSSException(errMsg.c_str(), this->retVal, this->minor_status, inputNameType.toGss());
  }
  this->outputName.setValue(name);
  key = GSSNameCache::instance()->store(this->outputName);
//   std::cout << "Storing key: " << key << std::endl;
  this->outputName.setKey(key);
  /* Cleanup */
  /* Return */
  
}

/* Example output:
 * 
 *   {
 *     "major_status": 0,
 *     "minor_status": 0,
 *     "gss_name": "base64_encoded_string"
 *   }
 * 
 */
JSONObject *GSSImportName::toJSON()
{
  /* Variables */
  JSONObject *values = new JSONObject();
  
  /* Error checking */
  
  /* Setup */
  
  /* Main */
  values->set("major_status", this->retVal);
  values->set("minor_status", this->minor_status);
  values->set("gss_name", this->outputName.getKey().c_str() );
  
  /* Cleanup */
  
  /* Return */
  return(values);
}

GSSImportName::GSSImportName ( gss_imp_name_type fn )
{
  // defaults
  function = fn;
  inputName = "";
  inputNameType.setValue(GSSBuffer("{ 1 2 840 113554 1 2 1 4 }"));
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
  if ( params->isNull() )
    return true;
  
  if ( !params->get("input_name").isNull() )
  {
    if ( params->get("input_name").isString() )
    {
      std::string input_name = params->get("input_name").string();
      this->inputName.setValue( input_name );
    }
  }
  
  if ( !params->get("input_name_type").isNull() )
  {
    if ( params->get("input_name_type").isString() )
    {
      std::string input_name_type = params->get("input_name_type").string();
      this->inputNameType.setValue( input_name_type );
    }
  }
  
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
