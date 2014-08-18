/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSPSEUDORANDOMTEST_H
#define GSSPSEUDORANDOMTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class GSSPseudoRandomTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( GSSPseudoRandomTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testEmptyCall );
  CPPUNIT_TEST( testConstructorWithJSONObject );
  CPPUNIT_TEST( testJSONMarshal );
  CPPUNIT_TEST_SUITE_END();

public:
    void setUp();
    
    void testJSONMarshal();
    void testEmptyCall();
    void testConstructor();
    void testConstructorWithJSONObject();
};

#endif // GSSPSEUDORANDOMTEST_H
