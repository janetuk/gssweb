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

#ifndef GSSGETMIC_H
#define GSSGETMIC_H

#include "GSSCommand.h"
#include "datamodel/GSSBuffer.h"
#include <gssapi/gssapi.h>

typedef OM_uint32 (KRB5_CALLCONV *gss_get_mic_type) (
    OM_uint32 *,        /* minor_status */
    gss_ctx_id_t,       /* context_handle */
    gss_qop_t,          /* qop_req */
    gss_buffer_t,       /* message_buffer */
    gss_buffer_t);      /* message_token */

class GSSGetMic : public GSSCommand
{
public:
    GSSGetMic( gss_get_mic_type fn = &gss_get_mic ) : function(fn) {};
    GSSGetMic( JSONObject *params, gss_get_mic_type fn = &gss_get_mic);
    ~GSSGetMic() {};
    virtual void execute();
    virtual JSONObject* toJSON();
    
    gss_get_mic_type getGSSFunction() const { return(function); }

    // Setters
    void setQopReq(gss_qop_t req) { this->qop_req = req; }
    void setInputMessage ( GSSBuffer* input ) { inputMessage.setValue(input->toString()); };
    void setContextHandle ( gss_ctx_id_t context_handle) { context = context_handle; }
    
    // Getters
    gss_ctx_id_t getContextHandle() const { return(this->context); }
    gss_qop_t    getQopReq() const { return(this->qop_req); }
    GSSBuffer    getInputMessage() const { return(this->inputMessage); }
    GSSBuffer    getOutputToken() const { return(this->outputToken); }
  
private:
    OM_uint32        retVal;
    OM_uint32        minor_status;

    gss_get_mic_type function;
    gss_ctx_id_t     context;
    gss_qop_t        qop_req;
    GSSBuffer        inputMessage;
    GSSBuffer        outputToken;
    
    bool loadParameters( JSONObject *params );
};

#endif // GSSGETMIC_H
