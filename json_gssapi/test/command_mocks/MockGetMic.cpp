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
