/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef MOCKGETMIC_H
#define MOCKGETMIC_H

#include "datamodel/GSSBuffer.h"
#include <gssapi/gssapi.h>

/*
 * OM_uint32 KRB5_CALLCONV
 * gss_get_mic(
 *    OM_uint32 *,         / * minor_status * /
 *     gss_ctx_id_t,       / * context_handle * /
 *     gss_qop_t,          / * qop_req * /
 *     gss_buffer_t,       / * message_buffer * /
 *     gss_buffer_t);      / * message_token * /
 */

class MockGetMic
{
public:
  static OM_uint32     retVal;
  static OM_uint32     minor_status;
  static gss_ctx_id_t  context_handle;
  static gss_qop_t     qop_req;
  static GSSBuffer     inputMessage;
  static GSSBuffer     outputToken;
  
  static void reset();

};

#endif // MOCKGETMIC_H
