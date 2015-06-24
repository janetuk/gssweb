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

#include "GSSOID.h"
#include "../GSSException.h"

GSSOID::GSSOID(std::string oid_str)
{
  init( GSSBuffer(oid_str) );
}

GSSOID::GSSOID(char *oid_str)
{
  init( GSSBuffer(oid_str) );
}
  
GSSOID::GSSOID(GSSBuffer oid_str)
{
  init(oid_str);
}

GSSOID::GSSOID(gss_OID gssoid)
{
  oid = gssoid;
  this->gssInternal = true;
}

GSSOID::GSSOID ( const GSSOID &gssoid )
{
  this->oid = gssoid.oid;
  this->gssInternal = true;
}

void GSSOID::release()
{
  OM_uint32 major, minor;
  if ( (!this->gssInternal) && (oid != GSS_C_NO_OID) )
  {
    major = gss_release_oid(&minor, &(this->oid));
    if (GSS_ERROR(major))
    {
      throw GSSException("Error in releasing a GSS OID", major, minor);
    }
  }
}

GSSOID::~GSSOID()
{
  this->release();
}
  
void GSSOID::init(GSSBuffer oid_str)
{
  /* Variables */
  OM_uint32 major, minor;
  
  /* Error checking */
  /* Setup */
  
  /* Main */
  major = gss_str_to_oid(&minor, oid_str.toGss(), &(this->oid));
  if (GSS_ERROR(major))
  {
    throw GSSException("Error creating GSS OID", major, minor);
  }
  this->gssInternal = false;
  
  /* Cleanup */
  /* Return */
}

std::string GSSOID::toString() const
{
  /* Variables */
  OM_uint32 major, minor;
  gss_buffer_desc buf;
  std::string ret;
  
  /* Error checking */
  if (oid->length == 0)
    return std::string("");
  /* Setup */
  /* Main */ 
  major = gss_oid_to_str(&minor, this->oid, &buf);
  if (GSS_ERROR(major))
  {
    throw GSSException("Error in creating a string representation of a GSS OID", 
		       major, minor);
  }
  ret = std::string((char *)buf.value);
  
  /* Cleanup */
  major = gss_release_buffer(&minor, &buf);
  if (GSS_ERROR(major))
  {
    throw GSSException("Error releasing a gss_buffer", 
		       major, minor);
  }
  
  /* Return */ 
  return(ret);
}

JSONObject *GSSOID::toJSONValue() const
{ 
  std::string val = this->toString();
  JSONObject *ret = new JSONObject( val.c_str() );
  return(ret); 
}

bool GSSOID::setValue ( GSSBuffer buf )
{
  init(buf);
  return true;
}

bool GSSOID::setValue ( gss_OID gssOID )
{
  oid = gssOID;
  this->gssInternal = true;
  return true;
}
