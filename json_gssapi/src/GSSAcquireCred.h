/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSACQUIRECREDCOMMAND_H
#define GSSACQUIRECREDCOMMAND_H

#include "GSSCommand.h"
#include "datamodel/GSSCredential.h"
#include "datamodel/GSSOIDSet.h"
#include "datamodel/GSSName.h"
#include <gssapi/gssapi.h>

class GSSAcquireCred :  GSSCommand
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

};

#endif // GSSACQUIRECREDCOMMAND_H
