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

#include <algorithm>
#include <string>

//#include <cppunit/TestFixture.h>
//#include <cppunit/extensions/HelperMacros.h>
#include <openssl/rand.h>

#include "cache/GSSContextCache.h"
#include "datamodel/GSSContext.h"
#include "command_mocks/MockPseudoRandom.h"
#include "GSSPseudoRandom.h"
#include "GSSPseudoRandomTest.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( GSSPseudoRandomTest );


// typedef OM_uint32 (*gss_pseudo_random_type) (
//     OM_uint32 *,        /* minor_status */
//     gss_ctx_id_t,       /* context */
//     int,                /* prf_key */
//     const gss_buffer_t, /* prf_in */
//     ssize_t,            /* desired_output_len */
//     gss_buffer_t);      /* prf_out */
OM_uint32 mock_gss_pseudo_random(
    OM_uint32 *minor_status,
    gss_ctx_id_t  context,
    int prf_key,
    const gss_buffer_t prf_in,
    ssize_t desired_output_len,
    gss_buffer_t prf_out)
{
  /* Variables */
  /* Error checking */
  /* Setup */
  /* Main */
  MockPseudoRandom::context_handle = context;
  MockPseudoRandom::key = prf_key;
  MockPseudoRandom::inputMessageBuffer.setValue(prf_in);
  MockPseudoRandom::desiredOutputLength = desired_output_len;
  
  *prf_out = *MockPseudoRandom::outputMessageBuffer.toGss();
  
  /* Cleanup */
  /* Return */
  return 0;
}


void GSSPseudoRandomTest::setUp()
{
  CppUnit::TestFixture::setUp();
  MockPseudoRandom::reset();
}


void GSSPseudoRandomTest::testConstructor()
{
  /* Variables */
  GSSPseudoRandom cmd = GSSPseudoRandom();
  
  /* Error checking */
  /* Setup */
  /* Main */
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The GSSPseudoRandom object has the wrong GSS function",
    (void *)&gss_pseudo_random,
    (void *)cmd.getGSSFunction()
  );
  
  /* Cleanup */
  /* Return */
}

/* 
 * Test that the command calls into gss_pseudo_random with
 * all of the correct values, and that the command reads
 * all of the appropriate values back out of it.
 */
void GSSPseudoRandomTest::testEmptyCall()
{
  /* Variables */
  GSSPseudoRandom cmd = GSSPseudoRandom(&mock_gss_pseudo_random);
  gss_ctx_id_t desiredContext;
  int prf_key;
  ssize_t desired_output_len;
  GSSBuffer prf_in((char *)"Input message");
  
  /* Error Checking */
  /* Setup */
  
  // Populate cmd with what will be used to call into gss_pseudo_random
    // taking the address of prf_key should be plenty random for testing the empty call.
  desiredContext = (gss_ctx_id_t)&prf_key;
  prf_key = rand();
  desired_output_len = (ssize_t)(rand() + 50);  // Must be at least fifty bytes long
  
  cmd.setContextHandle(desiredContext);
  cmd.setKey(prf_key);
  cmd.setInputMessage( prf_in );
  cmd.setDesiredOutputLength(desired_output_len);
  
  
  // Populate the mock with what will be returned from gss_pseudo_random
  MockPseudoRandom::minor_status = 0;
  MockPseudoRandom::retVal = 0;
  MockPseudoRandom::outputMessageBuffer.setValue((char *)"Output message");

  
  /* Main */
  cmd.execute();
  
  // Do we populate the call to gss_pseudo_random correctly?
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The requested GSS context handle is not correct",
    desiredContext,
    MockPseudoRandom::context_handle
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The requested pseudo random function key is not correct",
    prf_key,
    MockPseudoRandom::key
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The requested desired output length is not correct",
    desired_output_len,
    MockPseudoRandom::desiredOutputLength
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The requested input message is not correct",
    prf_in.toString(),
    MockPseudoRandom::inputMessageBuffer.toString()
  );
  
  // Do we read the results of gss_pseudo_random correctly?
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The requested output message is not correct",
    MockPseudoRandom::outputMessageBuffer.toString(),
    cmd.getOutputMessage().toString()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The return value is not correct",
    MockPseudoRandom::retVal,
    cmd.getRetVal()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The minor status value is not correct",
    MockPseudoRandom::minor_status,
    cmd.getMinorStatus()
  );
  
  
  
  /* Cleanup */
  /* Return */
}

/* JSON input
 * 
 * {"method":    "gss_pseudo_random",
 *  "arguments": 
 *   {
 *     "context_handle":     "context handle key",
 *     "prf_key":            ###,
 *     "prf_in":             "la la la input message",
 *     "desired_output_len": ####
 *   }
 * 
 */
void GSSPseudoRandomTest::testConstructorWithJSONObject()
{
  /* Variables */
  GSSContext context( (gss_ctx_id_t)( (long)0 | rand() ),
                     true );
  std::string key = GSSContextCache::instance()->store(context);
  
  std::string input = "{ \
         \"context_handle\": \"" + key + "\", \
         \"prf_key\": 1234567890, \
         \"prf_in\": \"mary had a little lamb\", \
         \"desired_output_len\": 256 \
    }";
  json_error_t jsonErr;
  JSONObject json = JSONObject::load(input.c_str(), 0, &jsonErr);
  
  GSSPseudoRandom cmd = GSSPseudoRandom(&json, &mock_gss_pseudo_random);

  /* Error Checking */
  /* Setup */
  /* Main */
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSWrap did not retrive the GSS context correctly",
    context.getContext(),
    cmd.getContextHandle()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSPseudoRandom did not parse the prf_key argument correctly.",
    1234567890,
    cmd.getKey()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSPseudoRandom did not parse the prf_in argument correctly.",
    std::string("mary had a little lamb"),
    cmd.getInputMessage().toString()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSPseudoRandom did not parse the desired_output_len argument correctly.",
    256,
    cmd.getDesiredOutputLength()
  );
  
  /* Cleanup */
  /* Return */
}

/* Desired JSON output:
 * 
 * {
 *  "command":       "gss_pseudo_random",
 *  "return_values": 
 *  {
 *      "major_status": 0,
 *      "minor_status": 0,
 *      "random_bytes": "asdf"
 *  }
 * }
 */
void GSSPseudoRandomTest::testJSONMarshal()
{

  /* Variables */
  std::string output("dns@google.com");
  JSONObject *result;
  GSSPseudoRandom cmd = GSSPseudoRandom(&mock_gss_pseudo_random);
  
  /* Error checking */
  /* Setup */
  MockPseudoRandom::minor_status = 0;
  MockPseudoRandom::retVal = 0;
  MockPseudoRandom::outputMessageBuffer.setValue(output);
  
  /* Main */
  cmd.execute();
  result = cmd.toJSON();
//   std::cout << "\nGSSWrap JSON: \n" << result->dump() << "\n";
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The return value was reported incorrectly",
    (int)MockPseudoRandom::retVal,
    (int)( (*result)["major_status"].integer() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The minor_status value was reported incorrectly",
    (int)MockPseudoRandom::minor_status,
    (int)( (*result)["minor_status"].integer() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The random bytes were reported incorrectly",
    MockPseudoRandom::outputMessageBuffer.toString(),
    std::string( (*result)["random_bytes"].string() )
  );
  
  
  /* Cleanup */
  /* Return */
}

  /* Variables */
  /* Error Checking */
  /* Setup */
  /* Main */
  /* Cleanup */
  /* Return */
