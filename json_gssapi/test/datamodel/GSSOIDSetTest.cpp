/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "GSSOIDSetTest.h"
#include <datamodel/GSSOIDSet.h>

CPPUNIT_TEST_SUITE_REGISTRATION( GSSOIDSetTest );

void GSSOIDSetTest::setUp()
{

}

void GSSOIDSetTest::tearDown()
{

}

void GSSOIDSetTest::testConstructor()
{
  /* Variables */
  /* Error checking */
  /* Setup */
  /* Main */
  CPPUNIT_ASSERT_NO_THROW_MESSAGE(
    "GSSOIDSet constructor without exceptions;",
    GSSOIDSet subject()
  );
  
  /* Cleanup */
  /* return */
  
}

void GSSOIDSetTest::testAddOID()
{
  /* Variables */
  GSSOIDSet subject;
  GSSOID    newOID((char *)"{1 3 6 1 5 5 2}");
  
  /* Error checking */
  /* Setup */
  /* Main */
  CPPUNIT_ASSERT_NO_THROW_MESSAGE(
    "Adding an OID to the set without an exception", 
    subject.addOID(newOID)
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "Counter is not incremented after appending an OID to a set",
    1,
    (int)(subject.toGss()->count)
  );
  
  /* Cleanup */
  /* return */
}


  /* Variables */
  /* Error checking */
  /* Setup */
  /* Main */
  /* Cleanup */
  /* return */
  