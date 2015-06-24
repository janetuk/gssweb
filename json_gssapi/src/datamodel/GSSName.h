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
#include "GSSBuffer.h"
#include "GSSOID.h"

#ifndef GSSNAME_H
#define GSSNAME_H


typedef OM_uint32 (KRB5_CALLCONV *gss_imp_name_type)(
    OM_uint32 *,        /* minor_status */
    gss_buffer_t,       /* input_name_buffer */
    gss_OID,            /* input_name_type(used to be const) */
    gss_name_t *);      /* output_name */


class GSSName {
public:
  GSSName() { name = GSS_C_NO_NAME; skipRelease = false; };
  GSSName(const GSSName& n);
  GSSName(gss_name_t gss_name, bool skipRelease = false);

  ~GSSName();
  
  GSSName& operator=(const GSSName& rhs);
  
  gss_name_t toGss() const { return(name); }
  std::string toString() const;
  
  bool setValue(gss_name_t newName, bool skipRelease = false);
  void setKey(std::string key) { this->hashKey = key; }
  std::string getKey() const { return this->hashKey; }
  
  OM_uint32 getMajorStatus() const { return this->major_status; }
  OM_uint32 getMinorStatus() const { return this->minor_status; }
  
private:
  OM_uint32 major_status, minor_status;
  gss_name_t name;
  gss_imp_name_type function;
  std::string hashKey;
  bool skipRelease;
  
  void init(const GSSBuffer namestr, GSSOID name_type, bool skipRelease, gss_imp_name_type fn);
  void release();
};


#endif // GSSNAME_H
