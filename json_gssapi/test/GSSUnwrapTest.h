/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSUNWRAPTEST_H
#define GSSUNWRAPTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class GSSUnwrapTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( GSSUnwrapTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testConstructorWithJSONObject );
  CPPUNIT_TEST( testEmptyCall );
  CPPUNIT_TEST( testJSONMarshal );
  CPPUNIT_TEST_SUITE_END();
  
public:
  void setUp();
  
  void testConstructor();
  void testConstructorWithJSONObject();
  void testEmptyCall();
  void testJSONMarshal();

};

#endif // GSSWRAPTEST_H