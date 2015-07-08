/*
 * Copyright (c) 2014, JANET(UK)
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

#ifndef GSSINITSECCONTEXT_H
#define GSSINITSECCONTEXT_H

#include "GSSCommand.h"
#include <datamodel/GSSContext.h>
#include <datamodel/GSSCredential.h>
#include <datamodel/GSSName.h>
#include <datamodel/GSSOID.h>
#include <gssapi.h>

typedef OM_uint32 (KRB5_CALLCONV *init_sec_context_type)(
    OM_uint32 *,        /* minor_status */
    gss_cred_id_t,      /* claimant_cred_handle */
    gss_ctx_id_t *,     /* context_handle */
    gss_name_t,         /* target_name */
    gss_OID,            /* mech_type (used to be const) */
    OM_uint32,          /* req_flags */
    OM_uint32,          /* time_req */
    gss_channel_bindings_t,     /* input_chan_bindings */
    gss_buffer_t,       /* input_token */
    gss_OID *,          /* actual_mech_type */
    gss_buffer_t,       /* output_token */
    OM_uint32 *,        /* ret_flags */
    OM_uint32 *         /* time_req */
);

class GSSInitSecContext : public GSSCommand
{
public:

    OM_uint32 retVal;
    OM_uint32 minor_status;
    gss_ctx_id_t context_handle;
    gss_name_t target_name;
    OM_uint32 req_flags;
    OM_uint32 time_req;
    gss_buffer_desc input_token;
    gss_buffer_desc output_token;
    OM_uint32 ret_flags;
    OM_uint32 time_rec;
    
  
    void execute();
    JSONObject *toJSON();
    GSSInitSecContext(init_sec_context_type fn = &gss_init_sec_context);
    GSSInitSecContext(JSONObject *params,  init_sec_context_type fn = &gss_init_sec_context);
    
    bool loadParameters(JSONObject *params);
    bool zeroOut(bool initialized = true);
    
    // MRW -- make more accessors, make vars w/accessors private, or get rid of them?
    // accessors
    OM_uint32 getReqFlags() { return req_flags; }
    OM_uint32 getTimeReq() { return time_req; }
    gss_ctx_id_t getContextHandle() { return context_handle; }
    init_sec_context_type getGSSFunction() { return function; }
    GSSOID getMechType() { return mechType; };
    GSSOID getActualMechType() { return actualMechType; };
    
    // complex accessors
    const char * getTargetDisplayName();
    
private:
    init_sec_context_type function;
    GSSContext context;
    GSSOID mechType;
    GSSOID actualMechType;
    GSSName targetName;
    std::string contextKey;
    GSSCredential claimantCred;

    JSONObject *values;
};

#endif // GSSINITSECCONTEXT_H
