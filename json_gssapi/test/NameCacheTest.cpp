/*
 * Copyright (c) 2014, 2015 JANET(UK)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of JANET(UK) nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
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
    OM_uint32 min, context;
    gss_buffer_desc buf;
    
    std::cout << "Error in importing name." << std::endl;
    gss_display_status(&min, major, GSS_C_GSS_CODE, GSS_C_NT_HOSTBASED_SERVICE, &context, &buf);
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
