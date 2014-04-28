/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef GSSBUFFERTEST_H
#define GSSBUFFERTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class GSSBufferTest : public CppUnit::TestFixture
{
  
  CPPUNIT_TEST_SUITE( GSSBufferTest );
  CPPUNIT_TEST( testStringConstructor );
  CPPUNIT_TEST_SUITE_END();

  
public:
    virtual void setUp();
    virtual void tearDown();
    
    virtual void testStringConstructor();
};

#endif // GSSBUFFERTEST_H
