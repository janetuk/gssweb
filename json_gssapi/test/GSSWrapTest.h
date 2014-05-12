/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSWRAPTEST_H
#define GSSWRAPTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class GSSWrapTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( GSSWrapTest );
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
