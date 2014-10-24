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

#ifndef GSSUNWRAP_H
#define GSSUNWRAP_H

#include "GSSCommand.h"
#include "datamodel/GSSBuffer.h"
#include <gssapi/gssapi.h>

typedef OM_uint32 (*gss_unwrap_type) (
    OM_uint32 *,        /* minor_status */
    gss_ctx_id_t,       /* context_handle */
    gss_buffer_t,       /* input_message_buffer */
    gss_buffer_t,       /* output_message_buffer */
    int *,              /* conf_state */
    gss_qop_t *);       /* qop_state */


class GSSUnwrap :  GSSCommand
{
public:
    GSSUnwrap( gss_unwrap_type fn = &gss_unwrap ) : function(fn) {};
    GSSUnwrap( JSONObject *params, gss_unwrap_type fn = &gss_unwrap);
    
    gss_unwrap_type getGSSFunction() { return(function); }
    void setContextHandle ( gss_ctx_id_t context_handle ) { context = context_handle; }
    void setInputMessage ( GSSBuffer* input ) { inputMessage.setValue(input->toString()); };
    
    gss_qop_t getQopState() const { return(qop_state); }
    int getConfState() const { return(conf_state); }
    GSSBuffer getInputMessage() const { return(inputMessage); }
    GSSBuffer getOutputMessage() const { return(outputMessage); }
    gss_ctx_id_t getContextHandle() const { return(context); }

    virtual void execute();  
    virtual JSONObject* toJSON();
    
private:
    gss_unwrap_type function;
    OM_uint32 retVal;
    OM_uint32 minor_status;
    gss_ctx_id_t context;
    gss_qop_t qop_state;
    int conf_state;
    
    GSSBuffer inputMessage;
    GSSBuffer outputMessage;
    
    bool loadParameters(JSONObject *params);
};

#endif // GSSUNWRAP_H
