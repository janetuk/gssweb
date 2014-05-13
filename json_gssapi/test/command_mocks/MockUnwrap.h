/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef MOCKUNWRAP_H
#define MOCKUNWRAP_H

#include "datamodel/GSSBuffer.h"
#include <gssapi/gssapi.h>

class MockUnwrap
{
public:
  static OM_uint32     retVal;
  static OM_uint32     minor_status;
  static gss_ctx_id_t  context_handle;
  static GSSBuffer     inputMessageBuffer;
  static GSSBuffer     outputMessageBuffer;
  static int           conf_state;
  static gss_qop_t     qop_state;
  
  static void reset();

};

#endif // MOCKUNWRAP_H
