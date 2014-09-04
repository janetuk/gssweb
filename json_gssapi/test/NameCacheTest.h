/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#ifndef NAMECACHETEST_H
#define NAMECACHETEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class NameCacheTest : public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( NameCacheTest );
  CPPUNIT_TEST( testStore );
//   CPPUNIT_TEST( testStoreWithKey );
//   CPPUNIT_TEST( testRetrieve );
  CPPUNIT_TEST_SUITE_END();

public:
    virtual void setUp();
    virtual void tearDown();
    
    void testStore();
//     void testStoreWithKey();
//     void testRetrieve();
};

#endif // NAMECACHETEST_H

