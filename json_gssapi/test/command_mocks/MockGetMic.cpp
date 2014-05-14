/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "MockGetMic.h"

/*
 * OM_uint32 KRB5_CALLCONV
 * gss_get_mic(
 *    OM_uint32 *,         / * minor_status * /
 *     gss_ctx_id_t,       / * context_handle * /
 *     gss_qop_t,          / * qop_req * /
 *     gss_buffer_t,       / * message_buffer * /
 *     gss_buffer_t);      / * message_token * /
 *
 * 
 *   class MockGetMic
 *   {
 *   public:
 *     static OM_uint32     retVal;
 *     static OM_uint32     minor_status;
 *     static gss_ctx_id_t  context_handle;
 *     static gss_qop_t     qop_req;
 *     static GSSBuffer     messageBuffer;
 *     static GSSBuffer     messageToken;
 *     
 *     static void reset();
 *
 *   };
 */

OM_uint32     MockGetMic::retVal;
OM_uint32     MockGetMic::minor_status;
gss_ctx_id_t  MockGetMic::context_handle;
gss_qop_t     MockGetMic::qop_req;
GSSBuffer     MockGetMic::inputMessage;
GSSBuffer     MockGetMic::outputToken;

void MockGetMic::reset()
{
  MockGetMic::retVal         = 0;
  MockGetMic::minor_status   = 0;
  MockGetMic::context_handle = GSS_C_NO_CONTEXT;
  MockGetMic::qop_req        = GSS_C_QOP_DEFAULT;
  MockGetMic::inputMessage   = GSSBuffer();
  MockGetMic::outputToken    = GSSBuffer();
}
