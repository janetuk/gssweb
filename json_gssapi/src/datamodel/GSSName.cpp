/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include "GSSName.h"
#include "../GSSException.h" 

void GSSName::init(const GSSBuffer namestr, GSSOID name_type, bool skipRelease, gss_imp_name_type fn)
{
  /* Variables */
  /* Error checking */
  /* Setup */
  /* Main */ 
  this->major_status = fn(&(this->minor_status), namestr.toGss(), name_type.toGss(), &name);
  if ( GSS_ERROR(this->major_status) )
  {
    std::string errMsg;
    errMsg += "Cannot import name: ";
    errMsg += namestr.toString();
    throw GSSException(errMsg.c_str(), this->major_status, this->minor_status, name_type.toGss());
  }
  this->skipRelease = skipRelease;
  
  /* Cleanup */
  /* Return */
}


GSSName::GSSName ( gss_name_t gss_name, bool skipRelease )
{ 
  name = gss_name; 
  this->skipRelease = skipRelease; 
  
}
 

void GSSName::release()
{
  /* Variables */
  OM_uint32 major, minor;
  
  /* Error checking */
  if (GSS_C_NO_NAME == name || skipRelease)
    return;
  
  /* Setup */
  /* Main */
  //  if (hashKey.length() > 0)
  //    std::cout << std::endl << "About to release name for key " << hashKey << std::endl;
  
  major = gss_release_name(&minor, &name);
  name = GSS_C_NO_NAME;
  hashKey = "";
  if ( GSS_ERROR(major) && !( major & GSS_S_BAD_NAME ) )
  {
    throw GSSException("Cannot free memory for a GSS name.", major, minor);
  }

  /* Cleanup */
  /* Return */
}

GSSName::GSSName ( const GSSName& n )
{
  this->hashKey = n.hashKey;
  if (GSS_C_NO_NAME == n.name)
  {
    major_status = 0;
    minor_status = 0;
    name = GSS_C_NO_NAME;
  }
  else
  {
    this->major_status = gss_duplicate_name(&minor_status, n.name, &name);
    if ( GSS_ERROR(major_status) )
    {
      throw GSSException("Cannot copy a GSS name.", major_status, minor_status);
    }
  }
  this->skipRelease = false;
}

GSSName::~GSSName()
{
  this->release();
}  

GSSName& GSSName::operator= ( const GSSName& rhs )
{
  if (rhs.toGss() != this->toGss())
  {
    this->function = rhs.function;
    this->minor_status = rhs.minor_status;
    this->major_status = rhs.major_status;
    this->name = rhs.name;
    this->hashKey = rhs.hashKey;
    this->skipRelease = rhs.skipRelease;
  }
  return *this;
}


std::string GSSName::toString() const
{
  /* Variables */
  OM_uint32 major, minor;
  gss_OID oid;
  GSSBuffer buf;
  
  /* Error checking */
  /* Setup */
  /* Main */ 
  major = gss_display_name(&minor, (this->name), buf.toGss(), &oid);
  if ( GSS_ERROR(major) )
  {
    throw GSSException("Cannot convert a GSS name to a string.", major, minor);
  }
  
  /* Cleanup */
  /* Return */
  return buf.toString();
}

bool GSSName::setValue ( gss_name_t newName, bool skipRelease )
{
  this->release();
  this->name = newName;
  this->skipRelease = skipRelease;
  return(true);
}
