/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSGETMICTEST_H
#define GSSGETMICTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class GSSGetMicTest : public CppUnit::TestFixture
{

  CPPUNIT_TEST_SUITE( GSSGetMicTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testConstructorWithJSONObject );
  CPPUNIT_TEST( testEmptyCall );
  CPPUNIT_TEST( testJSONMarshal );
  CPPUNIT_TEST_SUITE_END();

public:
    virtual void setUp();
    
    void testConstructor();
    void testConstructorWithJSONObject();
    void testEmptyCall();
    void testJSONMarshal();


};

#endif // GSSGETMICTEST_H
