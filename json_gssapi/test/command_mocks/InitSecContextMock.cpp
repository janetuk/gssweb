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

#include "InitSecContextMock.h"
#include <stdio.h>

OM_uint32 InitSecContextMock::retVal = 0;
OM_uint32 InitSecContextMock::minor_status = 0;
OM_uint32 InitSecContextMock::req_flags = 0;
OM_uint32 InitSecContextMock::time_req = 0;
OM_uint32 InitSecContextMock::ret_flags = 0;
OM_uint32 InitSecContextMock::time_rec = 0;

bool InitSecContextMock::visited = false;
bool InitSecContextMock::invalidContextHandle = false;

gss_cred_id_t InitSecContextMock::claimant_cred_handle = NULL;
gss_ctx_id_t  InitSecContextMock::context_handle = NULL;
gss_name_t    InitSecContextMock::target_name = NULL;
gss_OID       InitSecContextMock::mech_type = NULL;
gss_channel_bindings_t InitSecContextMock::input_chan_bindings = NULL;
gss_buffer_desc  InitSecContextMock::input_token;
gss_OID       InitSecContextMock::actual_mech_type = NULL;
gss_buffer_desc  InitSecContextMock::output_token;

void InitSecContextMock::reset()
{
  retVal = 0;
  minor_status = 0;
  nullify(claimant_cred_handle);
  nullify(context_handle);
  nullify(target_name);
  // Our current handling of OIDs means attempting to free one
  // will often result in a double-free or an attempt to free
  // a constant OID. For now, it is better to occasionally leak.
  // We should revisit this -Kevin
  mech_type = GSS_C_NO_OID;
  req_flags = 0;
  time_req = 0;
  nullify(input_chan_bindings);
  input_token.length = 0;
  input_token.value = NULL;
  actual_mech_type = GSS_C_NO_OID;
  output_token.length = 0;
  output_token.value = 0;
  ret_flags = 0;
  time_rec = 0;
  visited = false;
  invalidContextHandle = false;

}
