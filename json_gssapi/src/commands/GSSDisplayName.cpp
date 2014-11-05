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

#include "GSSDisplayName.h"
#include "cache/GSSNameCache.h"


using std::string;

GSSDisplayName::GSSDisplayName()
{

}

GSSDisplayName::~GSSDisplayName()
{

}

JSONObject* GSSDisplayName::toJSON()
{
  JSONObject *values = new JSONObject;
  values->set("major_status", (json_int_t)0);
  values->set("minor_status", (json_int_t)0);
  values->set("output_name", inputName.toString().c_str());
  
  return(values);
}

void GSSDisplayName::execute()
{
  // Execution is a side effect of the toJSON calls
}

bool GSSDisplayName::loadParameters ( const JSONObject* params )
{
  /* Variables */
  string key;
  
  /* Error checking */
  if ( params->isNull() )
    return true;
  
  /* Setup */
  
  /* Main processing */
  // MRW -- finish parsing all of the variables
  // claimant_cred_handle

  // input_name
  if (!(params->get("input_name").isNull() ||
	(params->get("input_name").isString() &&
	 string("") == params->get("input_name").string())))
  {
    if (params->get("input_name").isString())
    {
      key = params->get("input_name").string();
      this->inputName = GSSNameCache::instance()->retrieve( key.c_str() );
    }
  }
  return(true);  
}


GSSDisplayName::GSSDisplayName ( JSONObject *params )
{
  loadParameters(params);
}
