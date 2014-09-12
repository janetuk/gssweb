/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#ifndef GSSCREATESECCONTEXTCOMMAND_H
#define GSSCREATESECCONTEXTCOMMAND_H

#include "GSSCommand.h"
#include <datamodel/GSSContext.h>
#include <datamodel/GSSName.h>
#include <datamodel/GSSOID.h>
#include <gssapi.h>

class GSSCreateSecContextCommand : public GSSCommand
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
    GSSCreateSecContextCommand(void *fn = (void *)&gss_init_sec_context);
    GSSCreateSecContextCommand(JSONObject *params, void *fn = (void *)&gss_init_sec_context);
    
    bool loadParameters(JSONObject *params);
    bool zeroOut(bool initialized = true);
    
    
    // accessors
    OM_uint32 getReqFlags() { return req_flags; }
    OM_uint32 getTimeReq() { return time_req; }
    gss_ctx_id_t getContextHandle() { return context_handle; }
    void *getGSSFunction() { return function; }
    GSSOID getMechType() { return mechType; };
    GSSOID getActualMechType() { return actualMechType; };
    
    // complex accessors
    const char * getTargetDisplayName();
    
private:
    void *function;
    GSSContext context;
    GSSOID mechType;
    GSSOID actualMechType;
    GSSName targetName;
    std::string contextKey;
};

#endif // GSSCREATESECCONTEXTCOMMAND_H
