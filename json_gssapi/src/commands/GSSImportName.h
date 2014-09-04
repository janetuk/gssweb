/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#ifndef GSSIMPORTNAME_H
#define GSSIMPORTNAME_H

#include "GSSCommand.h"
#include "datamodel/GSSBuffer.h"
#include "datamodel/GSSName.h"

#include <gssapi.h>

class GSSImportName : public GSSCommand
{
public:
  GSSImportName(gss_imp_name_type fn = &gss_import_name);
  GSSImportName(JSONObject *params, gss_imp_name_type fn = &gss_import_name);
  
  void execute();
  JSONObject* toJSON();
  
  GSSBuffer getInputName() { return GSSBuffer(inputName); };
  GSSOID getInputNameType() { return GSSOID(inputNameType); };
  
  gss_imp_name_type getGSSFunction() { return function; };
  void setInputName ( std::string name ) { inputName.setValue(name); };
  void setInputNameType ( std::string type ) { inputNameType.setValue(type); };
  
private:
  gss_imp_name_type function;
  bool loadParameters(JSONObject *params);
  
  OM_uint32 retVal;
  OM_uint32 minor_status;
  GSSBuffer inputName;
  GSSOID    inputNameType;
  GSSName   outputName;
};

#endif // GSSIMPORTNAME_H
