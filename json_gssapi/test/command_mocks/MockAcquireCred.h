/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef MOCKACQUIRECRED_H
#define MOCKACQUIRECRED_H

#include <gssapi.h>

class MockAcquireCred
{
public:
  static OM_uint32        retVal;
  static OM_uint32        minor_status;
  static gss_name_t       desired_name;
  static OM_uint32        time_req;
  static gss_OID_set      desired_mechs;
  static gss_cred_usage_t cred_usage;
  static gss_cred_id_t    output_cred_handle;
  static gss_OID_set      actual_mechs;
  static OM_uint32        time_rec;
  
  static void reset();
};

#endif // MOCKACQUIRECRED_H
