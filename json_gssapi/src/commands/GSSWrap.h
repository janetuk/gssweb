/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSWRAP_H
#define GSSWRAP_H

#include <gssapi.h>

#include "GSSCommand.h"
#include "datamodel/GSSBuffer.h"

typedef OM_uint32 (*gss_wrap_type) (
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
