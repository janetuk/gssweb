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
  if (NULL != other)
  {
    for(i = 0; i < other->count; i++)
    {
      element = other->elements + i;
      this->addOID(element);
    }
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
  
