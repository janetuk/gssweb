/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSCONTEXT_H
#define GSSCONTEXT_H

#include <gssapi/gssapi.h>

#include "datamodel/GSSBuffer.h"

class GSSCreateSecContextTest;
class GSSContext
{
public:
    GSSContext();
    GSSContext ( const GSSContext& other );
    GSSContext ( gss_ctx_id_t ctxt, bool skipRelease = false ) : context(ctxt), skipRelease(skipRelease) {};
    ~GSSContext();
    GSSContext& operator= ( const GSSContext& other );
    
    void release(GSSBuffer *output);
    
    gss_ctx_id_t getContext() { return(context); };
    void setContext(gss_ctx_id_t ctxt, bool skipRelease = false);
    
private:
    gss_ctx_id_t context;
    bool skipRelease; 
};

#endif // GSSCONTEXT_H
