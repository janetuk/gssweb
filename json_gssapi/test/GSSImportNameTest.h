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

/*
 * GSSImportNameTest.h - Test the GSSImportName object.
 * Tests include:
 *   testConstructor - 
 *       Testing basic object creation
 *   testConstructorWithJSONObject - 
 *       Test object creation with a set of arguments
 *   testEmptyCall - 
 *       Basic test of the system, with an empty call to
 *       a mocked out gss_import_name function, to test
 *       whether the object passes and records arguments
 *       correctly
 *   testJSONMarshal - 
 *       test the JSON serialization of the object
 */

#ifndef GSSIMPORTNAMETEST_H
#define GSSIMPORTNAMETEST_H

// #include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

#include <gssapi.h>
#include "GSSInitSecContext.h"

#include <stdlib.h>

class GSSImportNameTest :  public CppUnit::TestFixture
{
  CPPUNIT_TEST_SUITE( GSSImportNameTest );
  CPPUNIT_TEST( testConstructor );
  CPPUNIT_TEST( testConstructorWithJSONObject );
  CPPUNIT_TEST( testEmptyCall );
  CPPUNIT_TEST( testJSONMarshal );
  CPPUNIT_TEST_SUITE_END();
  
  
  
public:
  void setUp();
  void tearDown();

  
  void testConstructor();
  void testConstructorWithJSONObject();
  void testEmptyCall();
  void testJSONMarshal();
};

#endif // GSSIMPORTNAMETEST_H
