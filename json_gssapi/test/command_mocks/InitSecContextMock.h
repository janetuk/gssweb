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
  static gss_buffer_desc        input_token;
  static gss_OID                actual_mech_type;
  static gss_buffer_desc        output_token;
  static OM_uint32              ret_flags;
  static OM_uint32              time_rec;
  
  static bool                   visited;
  static bool                   invalidContextHandle;
  
  template <class t>
    static void nullify(t *&ptr)
  {
    if (ptr)
    {
      free(ptr);
      ptr = NULL;
    }
  }

  static void reset();
};

#endif // INITSECCONTEXTMOCK_H
