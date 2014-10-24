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

#include "GSSImportName.h"
#include "GSSException.h"
#include <cache/GSSNameCache.h>
#include <datamodel/GSSDisplayStatus.h>


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
//   if ( GSS_ERROR(this->retVal) )
//   {
    JSONObject errors;
    GSSDisplayStatus ds(retVal, minor_status, inputNameType.toGss());
    errors.set("major_status_message", ds.getMajorMessage().c_str());
    errors.set("minor_status_message", ds.getMinorMessage().c_str());
    values->set("errors", errors);
//   }
  
  
  
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
  inputName = string("");
  inputNameType.setValue(GSSBuffer( string("{ 1 2 840 113554 1 2 1 4 }") ));
  values = new JSONObject();
}

GSSImportName::GSSImportName(JSONObject *params, gss_imp_name_type fn) : GSSCommand(params)
{
  /* Variables */
  /* Error checking */
  /* Setup */
  /* Main */
  values = new JSONObject();
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
