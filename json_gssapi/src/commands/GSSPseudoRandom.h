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

#ifndef GSSPSEUDORANDOMCOMMAND_H
#define GSSPSEUDORANDOMCOMMAND_H

#include <gssapi/gssapi.h>

#include "datamodel/GSSBuffer.h"
#include "GSSCommand.h"

typedef OM_uint32 (KRB5_CALLCONV *gss_pseudo_random_type) (
    OM_uint32 *,        /* minor_status */
    gss_ctx_id_t,       /* context */
    int,                /* prf_key */
    const gss_buffer_t, /* prf_in */
    ssize_t,            /* desired_output_len */
    gss_buffer_t);      /* prf_out */

class GSSPseudoRandom : public GSSCommand
{
public:
    GSSPseudoRandom(gss_pseudo_random_type fn = &gss_pseudo_random) : function(fn) {};
    GSSPseudoRandom(JSONObject *params, 
                           gss_pseudo_random_type fn = &gss_pseudo_random);
    void execute();
    JSONObject* toJSON();
    bool loadParameters(JSONObject *params);
    
    /* Setters */
    void setContextHandle ( gss_ctx_id_t desiredContext ) { context = (desiredContext); };
    void setKey ( int prf_in ) { key = (prf_in); };
    void setInputMessage ( const GSSBuffer prf_in ) { inputMessage.setValue(prf_in.toString()); };
    void setDesiredOutputLength ( int desired_output_len ) { desiredOutputLength =desired_output_len; };
    
    /* Getters */
    GSSBuffer getInputMessage() { return inputMessage; }
    GSSBuffer getOutputMessage() { return outputMessage; }
    OM_uint32 getRetVal() { return(retVal); }
    OM_uint32 getMinorStatus() { return(minor_status); }
    int       getKey() { return(key); }
    int       getDesiredOutputLength() { return(desiredOutputLength); }
    gss_ctx_id_t getContextHandle() const { return(this->context); }
    gss_pseudo_random_type getGSSFunction() { return function; };
    
private:
    OM_uint32 retVal, minor_status;
    gss_pseudo_random_type function;
    gss_ctx_id_t context;
    int key, desiredOutputLength;
    GSSBuffer inputMessage;
    GSSBuffer outputMessage;
};

#endif // GSSPSEUDORANDOMCOMMAND_H
