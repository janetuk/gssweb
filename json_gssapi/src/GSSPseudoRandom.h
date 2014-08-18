/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSPSEUDORANDOMCOMMAND_H
#define GSSPSEUDORANDOMCOMMAND_H

#include <openssl/rand.h>
#include <gssapi/gssapi.h>

#include "datamodel/GSSBuffer.h"
#include "GSSCommand.h"

typedef OM_uint32 (*gss_pseudo_random_type) (
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
