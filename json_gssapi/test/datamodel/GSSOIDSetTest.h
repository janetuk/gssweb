/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSOIDSETTEST_H
#define GSSOIDSETTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class GSSOIDSetTest : public CppUnit::TestFixture
{
  
  CPPUNIT_TEST_SUITE( GSSOIDSetTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testAddOID );
  CPPUNIT_TEST_SUITE_END();
  
public:
    virtual void setUp();
    virtual void tearDown();

    void testConstructor();
    void testAddOID();
};

#endif // GSSOIDSETTEST_H
