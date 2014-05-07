/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
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
}

GSSCredential& GSSCredential::operator= ( const GSSCredential& gsscred )
{
  this->credential = gsscred.credential;
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

