/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include "GSSName.h"
#include "../GSSException.h" 

void GSSName::init(GSSBuffer namestr, GSSOID name_type, gss_imp_name_type fn)
{
  /* Variables */
  OM_uint32 major, minor;
  /* Error checking */
  /* Setup */
  /* Main */ 
  major = gss_import_name(&minor, namestr.toGss(), name_type.toGss(), &name);
  if ( GSS_ERROR(major) )
  {
    std::string errMsg;
    errMsg += "Cannot import name: ";
    errMsg += namestr.toString();
    throw GSSException(errMsg.c_str(), major, minor, name_type.toGss());
  }
  this->function = fn;
  
  /* Cleanup */
  /* Return */
}



GSSName::GSSName(std::string namestr, GSSOID name_type, gss_imp_name_type fn)
{
  init(namestr, name_type, fn);
}

GSSName::GSSName(char *namestr,       GSSOID name_type, gss_imp_name_type fn)
{
  init(GSSBuffer(namestr), name_type, fn); 
}

GSSName::GSSName(GSSBuffer namestr,   GSSOID name_type, gss_imp_name_type fn)
{
  init(GSSBuffer(namestr), name_type, fn); 
}

GSSName::GSSName(std::string namestr, gss_OID name_type, gss_imp_name_type fn)
{
  init(GSSBuffer(namestr), GSSOID(name_type), fn); 
}

GSSName::GSSName(char *namestr,       gss_OID name_type, gss_imp_name_type fn)
{
  init(GSSBuffer(namestr), GSSOID(name_type), fn); 
}

GSSName::GSSName(GSSBuffer namestr,   gss_OID name_type, gss_imp_name_type fn)
{
  init(namestr, GSSOID(name_type), fn); 
}

GSSName::GSSName(std::string namestr, std::string name_type, gss_imp_name_type fn)
{
  init(GSSBuffer(namestr), GSSOID(name_type), fn); 
}

GSSName::GSSName(char *namestr,       std::string name_type, gss_imp_name_type fn)
{
  init(GSSBuffer(namestr), GSSOID(name_type), fn); 
}

GSSName::GSSName(GSSBuffer namestr,   std::string name_type, gss_imp_name_type fn)
{
  init(namestr, GSSOID(name_type), fn); 
}


GSSName::~GSSName()
{
  /* Variables */
  OM_uint32 major, minor;
  
  /* Error checking */
  /* Setup */
  /* Main */ 
  major = gss_release_name(&minor, &name);
  if ( GSS_ERROR(major) && !( major & GSS_S_BAD_NAME ) )
  {
    throw GSSException("Cannot free memory for a GSS name.", major, minor);
  }

  /* Cleanup */
  /* Return */
}  


std::string GSSName::toString()
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
