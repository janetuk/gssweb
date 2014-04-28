/*
 * Copyright (c) 2014 Painless Security LLC
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 * GSSImportNameTest.h - Test the GSSImportName object.
 * Tests include:
 *   testConstructor - 
 *       Testing basic object creation
 *   testConstructorWithJSONObject - 
 *       Test object creation with a set of arguments
 *   testEmptyCall - 
 *       Basic test of the system, with an empty call to
 *       a mocked out gss_import_name function, to test
 *       whether the object passes and records arguments
 *       correctly
 *   testJSONMarshal - 
 *       test the JSON serialization of the object
 */

#ifndef GSSIMPORTNAMETEST_H
#define GSSIMPORTNAMETEST_H

// #include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gssapi.h>
#include "GSSCreateSecContextCommand.h"

#include <stdlib.h>

class GSSImportNameTest :  public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( GSSImportNameTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testConstructorWithJSONObject );
  CPPUNIT_TEST( testEmptyCall );
  CPPUNIT_TEST( testJSONMarshal );
  CPPUNIT_TEST_SUITE_END();
  
  
  
public:
  void setUp();
  void tearDown();

  
  void testConstructor();
  void testConstructorWithJSONObject();
  void testEmptyCall();
  void testJSONMarshal();
};

#endif // GSSIMPORTNAMETEST_H
