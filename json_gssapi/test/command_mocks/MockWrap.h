/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef MOCKWRAP_H
#define MOCKWRAP_H

#include "datamodel/GSSBuffer.h"

class MockWrap
{
public:
  static OM_uint32     retVal;
  static OM_uint32     minor_status;
  static gss_ctx_id_t  context_handle;
  static int           conf_req_flag;
  static gss_qop_t     qop_req;
  static GSSBuffer     inputMessageBuffer;
  static int           conf_state;
  static GSSBuffer     outputMessageBuffer;
  
  static void reset();
};

#endif // MOCKWRAP_H
