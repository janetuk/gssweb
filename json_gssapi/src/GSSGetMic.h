/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSGETMIC_H
#define GSSGETMIC_H

#include "GSSCommand.h"
#include "datamodel/GSSBuffer.h"
#include <gssapi/gssapi.h>

typedef OM_uint32 (*gss_get_mic_type) (
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
