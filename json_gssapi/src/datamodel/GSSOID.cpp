/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
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
