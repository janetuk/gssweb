/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSACQUIRECREDTEST_H
#define GSSACQUIRECREDTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class GSSAcquireCredTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( GSSAcquireCredTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testConstructorWithJSONObject );
  CPPUNIT_TEST( testEmptyCall );
  CPPUNIT_TEST( testJSONMarshal );
  CPPUNIT_TEST_SUITE_END();

public:
    virtual void setUp();
    virtual void tearDown();

    void testConstructor();
    void testConstructorWithJSONObject();
    void testEmptyCall();
    void testJSONMarshal();

};

#endif // GSSACQUIRECREDTEST_H
