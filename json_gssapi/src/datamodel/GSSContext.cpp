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

#include "GSSContext.h"
#include "GSSException.h"

#include <gssapi/gssapi.h>

GSSContext::GSSContext()
{
  skipRelease = false;
  context = GSS_C_NO_CONTEXT;
}

GSSContext::GSSContext ( const GSSContext& other )
{
  // Variables
  // Error checking
  // Setup
  // Main
  skipRelease = true;
  this->context = other.context;
  // Cleanup
  // Return

}

GSSContext::~GSSContext()
{
  // Variables
  GSSBuffer output;
  // Error checking
  // Setup
  // Main
  release(&output);

  // Cleanup
  // Would love to do something useful with outputToken, but in a destructor that's
  // not particularly meaningful.
  // Return
}

void GSSContext::setContext ( gss_ctx_id_t ctxt, bool skipRelease )
{
  context = ctxt; 
  this->skipRelease = skipRelease;
}


GSSContext& GSSContext::operator= ( const GSSContext& other )
{
  this->context = other.context;
  this->skipRelease = true;
  
  return *this;
}

void GSSContext::release(GSSBuffer *output)
{
  // Variables
  gss_buffer_desc outputToken;
  OM_uint32 major, minor;
  
  // Error checking
  if (GSS_C_NO_CONTEXT == context || skipRelease)
    return;
  
  // Setup

  // Main
  major = gss_delete_sec_context(&minor, &context, &outputToken);
  if (GSS_ERROR(major))
  {
    throw GSSException("Error in releasing a GSS context", major, minor);
  }

  output->setValue(&outputToken);
  
  // Cleanup
  // Return
}


  // Variables
  // Error checking
  // Setup
  // Main
  // Cleanup
  // Return
