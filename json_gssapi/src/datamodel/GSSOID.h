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

#include <string>
#include "gssapi.h"
#include "utils/util_json.h"

#include "GSSBuffer.h"

#ifndef GSSOID_H
#define GSSOID_H

static  const gss_OID_desc eap_mech_oid = {
  /* 1.3.6.1.5.5.15.1.1  */
  8, (void *) "\x2B\x06\x01\x05\x05\x0f\x01\x01" };


class GSSOID { 
public:
  GSSOID() { oid = GSS_C_NO_OID; gssInternal = true; };
  GSSOID(char *oid_str);
  GSSOID(std::string oid_str);
  GSSOID(GSSBuffer oid_str);
  GSSOID(gss_OID gssoid);
  GSSOID(const GSSOID &gssoid);
  ~GSSOID();
  
  gss_OID toGss() const { return(oid); };
  std::string toString() const;
  
  bool setValue(GSSBuffer buf);
  bool setValue(gss_OID   gssOID);

  JSONObject *toJSONValue() const;
  
  bool matchesBaseOID ( const gss_OID_desc &base_oid );
  bool isGssEapMech() { return( matchesBaseOID( eap_mech_oid ) ); }
  
private:
  gss_OID oid;
  bool gssInternal;
  
  void init(GSSBuffer oid_str);
  void release();
};


#endif // GSSOID_H
