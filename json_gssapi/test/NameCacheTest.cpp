/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include <string>
#include <iostream>

#include "NameCacheTest.h"
#include "cache/GSSNameCache.h"
#include "datamodel/GSSName.h"
#include <GSSImportName.h>

CPPUNIT_TEST_SUITE_REGISTRATION( NameCacheTest );

void NameCacheTest::setUp()
{

}

void NameCacheTest::tearDown()
{

}

void NameCacheTest::testStore()
{
  /* Variables      */
  std::string  key;
  GSSName      source( (gss_name_t)rand(), true), target;
  
  /* Error checking */
  /* Setup          */

  
  /* Main           */
  
  // Store the data
  key = GSSNameCache::instance()->store(source);
  
  // verify that the data can be retrieved.
  target = GSSNameCache::instance()->retrieve(key);
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The name cache did not store and retrieve the same data",
    source.toGss(),
    target.toGss()
  );

  /* Cleanup        */
  /* Return         */
  
}



  /* Variables      */
  /* Error checking */
  /* Setup          */
  /* Main           */
  /* Cleanup        */
  /* Return         */
