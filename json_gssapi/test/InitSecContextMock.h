/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#ifndef INITSECCONTEXTMOCK_H
#define INITSECCONTEXTMOCK_H

#include <gssapi.h>
#include <stdlib.h>
#include <iostream>

class InitSecContextMock
{
public:
  static OM_uint32              retVal;
  static OM_uint32              minor_status;
  static gss_cred_id_t          claimant_cred_handle;
  static gss_ctx_id_t           context_handle;
  static gss_name_t             target_name;
  static gss_OID                mech_type;
  static OM_uint32              req_flags;
  static OM_uint32              time_req;
  static gss_channel_bindings_t input_chan_bindings;
  static gss_buffer_t           input_token;
  static gss_OID                actual_mech_type;
  static gss_buffer_t           output_token;
  static OM_uint32              ret_flags;
  static OM_uint32              time_rec;
  
  static bool                   visited;
  static bool                   invalidContextHandle;
  
  static void nullify(void **ptr)
  {
    if (!ptr)
    {
      std::cout << std::endl << "Nullify called with a void ** that is NULL at the top level" << std::endl;
      return;
    }
    if (*ptr)
    {
      free(*ptr);
      *ptr = NULL;
    }
  }

  static void reset();
};

#endif // INITSECCONTEXTMOCK_H
