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

#include "GSSOIDSetTest.h"
#include <datamodel/GSSOIDSet.h>

CPPUNIT_TEST_SUITE_REGISTRATION( GSSOIDSetTest );

void GSSOIDSetTest::setUp()
{

}

void GSSOIDSetTest::tearDown()
{

}

void GSSOIDSetTest::testConstructor()
{
  /* Variables */
  /* Error checking */
  /* Setup */
  /* Main */
  CPPUNIT_ASSERT_NO_THROW_MESSAGE(
    "GSSOIDSet constructor without exceptions;",
    GSSOIDSet subject()
  );
  
  /* Cleanup */
  /* return */
  
}

void GSSOIDSetTest::testAddOID()
{
  /* Variables */
  GSSOIDSet subject;
  GSSOID    newOID((char *)"{1 3 6 1 5 5 2}");
  
  /* Error checking */
  /* Setup */
  /* Main */
  CPPUNIT_ASSERT_NO_THROW_MESSAGE(
    "Adding an OID to the set without an exception", 
    subject.addOID(newOID)
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "Counter is not incremented after appending an OID to a set",
    1,
    (int)(subject.toGss()->count)
  );
  
  /* Cleanup */
  /* return */
}


  /* Variables */
  /* Error checking */
  /* Setup */
  /* Main */
  /* Cleanup */
  /* return */
  