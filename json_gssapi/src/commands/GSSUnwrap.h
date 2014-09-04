/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
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
