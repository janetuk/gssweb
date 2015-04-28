/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSCREDENTIAL_H
#define GSSCREDENTIAL_H

#include "utils/util_json.h"
#include <gssapi/gssapi.h>
#include <string>

typedef OM_uint32 (KRB5_CALLCONV *gss_acq_cred_type)(
    OM_uint32 *,        /* minor_status */
    gss_name_t,         /* desired_name */
    OM_uint32,          /* time_req */
    gss_OID_set,        /* desired_mechs */
    gss_cred_usage_t,   /* cred_usage */
    gss_cred_id_t *,    /* output_cred_handle */
    gss_OID_set *,      /* actual_mechs */
    OM_uint32 *);       /* time_rec */



class GSSCredential
{
public:
    GSSCredential();
    GSSCredential(const GSSCredential &other);
    GSSCredential(gss_cred_id_t cred) : credential(cred) {};
    ~GSSCredential();
    
    GSSCredential& operator= (const GSSCredential &gsscred);
    
    gss_cred_id_t toGss() const { return(credential); }
    
    void setValue(gss_cred_id_t cred) { this->credential = cred; }
    JSONObject *toJSONValue() { return( new JSONObject("not now") ); }
    
private:
    gss_cred_id_t credential;
    
};

#endif // GSSCREDENTIAL_H
