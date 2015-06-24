/*
 * Copyright (c) 2014, 2015 JANET(UK)
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
