/*
 * Copyright (c) 2014, JANET(UK)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of JANET(UK) nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
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
  
  JSONObject *values;
};

#endif // GSSIMPORTNAME_H
