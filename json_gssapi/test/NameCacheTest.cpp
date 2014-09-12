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
  gss_name_t src;
  std::string  key;
  GSSName      source, target;
  OM_uint32 major, minor;
  
  /* Error checking */
  /* Setup          */
  major = gss_import_name(&minor, GSSBuffer((char *)"HTTP@localhost").toGss(), GSS_C_NT_HOSTBASED_SERVICE, &src);
  if (GSS_ERROR(major))
  {
    OM_uint32 maj, min, context;
    gss_buffer_desc buf;
    
    std::cout << "Error in importing name." << std::endl;
    maj = gss_display_status(&min, major, GSS_C_GSS_CODE, GSS_C_NT_HOSTBASED_SERVICE, &context, &buf);
    std::cout << "  message: " << (char *)buf.value << std::endl;
  }
  CPPUNIT_ASSERT_MESSAGE(
    "Could not generate a name to test storing into the cache.",
    !GSS_ERROR(major)
  );
  source.setValue(src);
  
  /* Main           */
  
  // Store the data
  key = GSSNameCache::instance()->store(source);
  
  // verify that the data can be retrieved.
  target = GSSNameCache::instance()->retrieve(key);
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The name cache did not store and retrieve the same data",
    source.toString(),
    target.toString()
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
