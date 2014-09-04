/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
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
