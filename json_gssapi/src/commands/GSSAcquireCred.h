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

#ifndef GSSACQUIRECREDCOMMAND_H
#define GSSACQUIRECREDCOMMAND_H

#include "GSSCommand.h"
#include "datamodel/GSSCredential.h"
#include "datamodel/GSSOIDSet.h"
#include "datamodel/GSSName.h"
#include <gssapi/gssapi.h>

class GSSAcquireCred : public GSSCommand
{
public:
    GSSAcquireCred( gss_acq_cred_type fn = &gss_acquire_cred );
    GSSAcquireCred ( const GSSAcquireCred& other );
    GSSAcquireCred ( JSONObject *params, gss_acq_cred_type fn = &gss_acquire_cred );
    ~GSSAcquireCred();
    
    GSSAcquireCred& operator= ( const GSSAcquireCred& other );
    
    virtual void execute();
    virtual JSONObject* toJSON();
    bool loadParameters(JSONObject *params);
    
    gss_acq_cred_type getGSSFunction() { return function; };
    
    void addDesiredMech ( const GSSOID &newOID ) { this->desiredMechs.addOID(newOID.toGss()); }
    
    // Getters & Setters
    void setDesiredName(const GSSName &name) { this->desired_name = name; }
    void setTimeReq(OM_uint32 request) { this->time_req = request; }
    void setCredUsage ( OM_uint32 credential_usage ) {this->cred_usage = credential_usage; }
    
    GSSName getDesiredName() { return(desired_name); }
    OM_uint32 getTimeReq() { return(time_req); }
    gss_cred_id_t getOutputCredHandle() const { return( cred.toGss() ); }
    OM_uint32 getRetVal() { return(retVal); }
    OM_uint32 getMinorStatus() { return(minor_status); }
    gss_OID_set getDesiredMechs() const { return(desiredMechs.toGss()); }
    gss_cred_usage_t getCredUsage() const { return(cred_usage); }
    gss_OID_set getActualMechs() const { return( actualMechs.toGss() ); }
    OM_uint32 getTimeRec() { return(time_rec); }
    
private:
    GSSName          desired_name;
    GSSCredential    cred;
    OM_uint32        time_req, time_rec;
    GSSOIDSet        desiredMechs;
    GSSOIDSet        actualMechs;
    gss_cred_usage_t cred_usage;
    OM_uint32        retVal, minor_status;
    
    gss_acq_cred_type function;
    
    JSONObject *values;

};

#endif // GSSACQUIRECREDCOMMAND_H
