/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "GSSOIDSet.h"
#include "../GSSException.h"

#include <cstring>

GSSOIDSet::GSSOIDSet( )
{
  init();
}

void GSSOIDSet::init()
{
  /* Variables */
  OM_uint32 major, minor;
  
  /* Error checking */
  /* Setup */
  /* Main */
  major = gss_create_empty_oid_set(&minor, &(this->set));
  
  // How the heck would this happen, anyway?
  if (GSS_ERROR(major))
    throw GSSException("Could not create an empty OID set.", major, minor);
  
  /* Cleanup */
  /* return */
}

GSSOIDSet::~GSSOIDSet()
{
  this->releaseOIDSet();
}

void GSSOIDSet::releaseOIDSet()
{
  /* Variables */
  OM_uint32 major, minor;
  
  /* Error checking */
  /* Setup */
  /* Main */
  major = gss_release_oid_set(&minor, &(this->set));
  if (GSS_ERROR(major))
    throw GSSException("Error in releasing memory for an OID set", major, minor);
  /* Cleanup */
  /* return */
}

GSSOIDSet& GSSOIDSet::operator= ( const gss_OID_set other )
{
  /* Variables */
  OM_uint32 i;
  gss_OID   element;
  
  /* Error checking */
  /* Setup */
  /* Main */
  this->releaseOIDSet();
  this->init();
  for(i = 0; i < other->count; i++)
  {
    element = other->elements + i;
    this->addOID(element);
  }
  
  /* Cleanup */
  /* return */
  return(*this);
}

void GSSOIDSet::addOID ( const GSSOID other )
{
  this->addOID(other.toGss());
}

void GSSOIDSet::addOID( const gss_OID other )
{
  /* Variables */
  OM_uint32 major, minor;
  
  /* Error checking */
  /* Setup */
  major = gss_add_oid_set_member(&minor, other, &(this->set) );
  if (GSS_ERROR(major))
    throw GSSException("Error encountered while trying to add another OID to a set", major, minor);

  /* Cleanup */
  /* return */
}


bool GSSOIDSet::includes ( const gss_OID oid ) const
{
  /* Variables */
  OM_uint32 major, minor;
  int present;
  
  /* Error checking */
  /* Setup */
  /* Main */
  major = gss_test_oid_set_member(&minor, oid, this->set, &present);
  if (GSS_ERROR(major))
    /* How? */
    throw GSSException("Cannot check if an OID is included in a set.", major, minor);
  
  /* Cleanup */
  /* return */
  return (present == 1);
}

bool GSSOIDSet::includes ( const GSSOID &oid ) const
{
  return(this->includes( oid.toGss() ) );
}

/*
 * Desired JSON Output:
 * 
 * [
 *   "{ 1 2 3 4 }",
 *   "{ 5 6 7 8 }"
 * ]
 * 
 */
JSONObject* GSSOIDSet::toJSONValue() const
{
  /* Variables */
  JSONObject  jsonArray = JSONObject::array();
  JSONObject *ret, *temp;
  OM_uint32   index;
  /* Error checking */
  /* Setup */
  /* Main */

  for (index = 0; index < set->count; index++)
  {
    GSSOID m( set->elements + index );
    temp = m.toJSONValue();
    jsonArray.append( *temp );
  }
  
  ret = new JSONObject(jsonArray);
  
  /* Cleanup */
  /* return */
  return(ret);
}


  /* Variables */
  /* Error checking */
  /* Setup */
  /* Main */
  /* Cleanup */
  /* return */
  