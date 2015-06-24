/*
 * Copyright (c) 2014, 2015 JANET(UK)
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

#include "MockAcquireCred.h"

/*
 * class MockAcquireCred
{
public:
  static OM_uint32        retVal;
  static OM_uint32        minor_status;
  static gss_name_t       desired_name;
  static OM_uint32        time_req;
  static gss_OID_set      desired_mechs;
  static gss_cred_usage_t cred_usage;
  static cred_id_t       *output_cred_handle;
  static gss_OID_set      actual_mechs;
  static OM_uint32       *time_rec;
  
  static void reset();
};
*/

OM_uint32        MockAcquireCred::retVal;
OM_uint32        MockAcquireCred::minor_status;
gss_name_t       MockAcquireCred::desired_name;
OM_uint32        MockAcquireCred::time_req;
gss_OID_set      MockAcquireCred::desired_mechs;
gss_cred_usage_t MockAcquireCred::cred_usage;
gss_cred_id_t    MockAcquireCred::output_cred_handle;
gss_OID_set      MockAcquireCred::actual_mechs;
OM_uint32        MockAcquireCred::time_rec;
  
void MockAcquireCred::reset()
{
  retVal = 0;
  minor_status = 0;
  desired_name = GSS_C_NO_NAME;
  time_req = 0;
  desired_mechs = GSS_C_NO_OID_SET;
  cred_usage = 0;
  output_cred_handle = GSS_C_NO_CREDENTIAL;
  actual_mechs = GSS_C_NO_OID_SET;
  time_rec = 0;
}

