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
  GSSImportName(gss_imp_name_type fn = &gss_import_name) : function(fn) {};
  GSSImportName(JSONObject *params, gss_imp_name_type fn = &gss_import_name);
  
  void execute();
  JSONObject* toJSON();
  
  GSSBuffer getInputName() { return GSSBuffer(inputName); }
  GSSBuffer getInputNameType() { return GSSBuffer(inputNameType); }
  
  gss_imp_name_type getGSSFunction() { return function; }
  
private:
  gss_imp_name_type function;
  bool loadParameters(JSONObject *params);
  GSSBuffer inputName;
  GSSBuffer inputNameType;

};

#endif // GSSIMPORTNAME_H
