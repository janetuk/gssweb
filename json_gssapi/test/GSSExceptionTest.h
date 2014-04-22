/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#ifndef GSSEXCEPTIONTEST_H
#define GSSEXCEPTIONTEST_H

// #include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include "GSSException.h"


class GSSExceptionTest :  public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( GSSExceptionTest );
  CPPUNIT_TEST( testWithMessage );
  CPPUNIT_TEST( testWithMessageAndMajor );
  CPPUNIT_TEST( testWithMessageAndMajorAndMinor );
  CPPUNIT_TEST( testWithMessageAndMajorAndMinorAndMechanism );
  CPPUNIT_TEST_SUITE_END();
  
  
  
public:
  void setUp();
  void tearDown();

  
  void testWithMessage();
  void testWithMessageAndMajor();
  void testWithMessageAndMajorAndMinor();
  void testWithMessageAndMajorAndMinorAndMechanism();
  
};


#endif // GSSEXCEPTIONTEST_H
