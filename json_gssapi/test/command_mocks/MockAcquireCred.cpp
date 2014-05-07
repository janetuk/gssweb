/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
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

