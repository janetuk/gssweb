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

#include "GSSCredential.h"
#include "../GSSException.h"

GSSCredential::GSSCredential()
{
  this->credential = GSS_C_NO_CREDENTIAL;
}

GSSCredential::GSSCredential ( const GSSCredential& other )
{
  this->credential = other.credential;
  this->hashKey = other.hashKey;
}

GSSCredential& GSSCredential::operator= ( const GSSCredential& gsscred )
{
  this->credential = gsscred.credential;
  this->hashKey = "";
  return(*this);
}

GSSCredential::~GSSCredential()
{
  /* Variables */
  OM_uint32 major, minor;
    
  /* Error checking */
  if (this->credential == GSS_C_NO_CREDENTIAL)
    return;
  
  /* Setup */
  
  /* Main */
  major = gss_release_cred(&minor, &(this->credential));
  
  /* Cleanup */
  if (GSS_ERROR(major))
    throw GSSException("Could not free the GSS credential", major, minor);
  
  /* Return */
}

