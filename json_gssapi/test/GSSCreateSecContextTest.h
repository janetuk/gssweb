/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#ifndef GSSCREATESECCONTEXTTEST_H
#define GSSCREATESECCONTEXTTEST_H

// #include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gssapi.h>
#include "GSSCreateSecContextCommand.h"

#include <stdlib.h>

class GSSCreateSecContextTest :  public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( GSSCreateSecContextTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testConstructorWithJSONObject );
  CPPUNIT_TEST( testEmptyCall );
  CPPUNIT_TEST_SUITE_END();
  
  
  
public:
  void setUp();
  void tearDown();

  
  void testConstructor();
  void testConstructorWithJSONObject();
  void testEmptyCall();
  
private:
  GSSCreateSecContextCommand command;
};

#endif // GSSCREATESECCONTEXTTEST_H
