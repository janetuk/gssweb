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

#ifndef GSSWRAP_H
#define GSSWRAP_H

#include <gssapi.h>

#include "GSSCommand.h"
#include "datamodel/GSSBuffer.h"

typedef OM_uint32 (KRB5_CALLCONV *gss_wrap_type) (
    OM_uint32 *,        /* minor_status */
    gss_ctx_id_t,       /* context_handle */
    int,                /* conf_req_flag */
    gss_qop_t,          /* qop_req */
    gss_buffer_t,       /* input_message_buffer */
    int *,              /* conf_state */
    gss_buffer_t);      /* output_message_buffer */


class GSSWrap :  GSSCommand
{
public:
    GSSWrap( gss_wrap_type fn = &gss_wrap ) : function(fn) {};
    GSSWrap( JSONObject *params, gss_wrap_type fn = &gss_wrap);
    ~GSSWrap();
    virtual void execute();
    virtual JSONObject* toJSON();
    
    gss_wrap_type getGSSFunction() const { return(function); }
    
    void setConfReq(int conf_req_flag) { conf_req = conf_req_flag; }
    void setContextHandle ( gss_ctx_id_t context_handle ) { context = context_handle; }
    void setQopReq ( int arg1 ) { qop_req = arg1; }
    void setInputMessage ( GSSBuffer* input ) { inputMessage.setValue(input->toString()); };
    
    int getConfState () const { return(conf_state); }
    GSSBuffer getOutputMessage () const { return(outputMessage); }
    int getConfReq() const { return(conf_req); }
    gss_qop_t getQopReq() const { return(qop_req); }
    GSSBuffer getInputMessage () const { return(inputMessage); }
    gss_ctx_id_t getContext() const { return(context); }
    
private:
    gss_wrap_type function;
    int conf_req;
    gss_ctx_id_t context;
    gss_qop_t qop_req;
    GSSBuffer inputMessage;
    GSSBuffer outputMessage;
    
  
    OM_uint32 retVal;
    OM_uint32 minor_status;
    int conf_state;

    bool loadParameters(JSONObject *params);
};

#endif // GSSWRAP_H
