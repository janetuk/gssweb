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

#include "GSSUnwrapTest.h"
#include "command_mocks/MockUnwrap.h"
#include "GSSUnwrap.h"
#include <datamodel/GSSContext.h>
#include <cache/GSSContextCache.h>
#include <gssapi/gssapi.h>


CPPUNIT_TEST_SUITE_REGISTRATION( GSSUnwrapTest );

/* 
 * a mock of the gss_import_name call
 * 
 * Basically, just copy the arguments over to/from the
 * MockUnwrap global object
 */

static OM_uint32 KRB5_CALLCONV
mock_unwrap(
    OM_uint32    *minor_status,
    gss_ctx_id_t  context_handle,
    gss_buffer_t  input_message_buffer,
    gss_buffer_t  output_message_buffer,
    int          *conf_state,
    gss_qop_t    *qop_state)
{
  /* Error checking */
  /* Variables */
  
  /* Setup */
  
  /* Main */
  // Copy our input from the appropriate parameters to MockUnwrap
  MockUnwrap::context_handle = context_handle;
  MockUnwrap::inputMessageBuffer.setValue(input_message_buffer);
  
  
  // copy our output to the appropriate parameters
  *minor_status = MockUnwrap::minor_status;
  *conf_state = MockUnwrap::conf_state;
  *qop_state = MockUnwrap::qop_state;
  *output_message_buffer = *MockUnwrap::outputMessageBuffer.toGss();
  
  /* Cleanup */
  /* return */
  return MockUnwrap::retVal;
}

void GSSUnwrapTest::setUp()
{
  CppUnit::TestFixture::setUp();
  MockUnwrap::reset();
}

void GSSUnwrapTest::testConstructor()
{
  /* Variables */
  GSSUnwrap cmd = GSSUnwrap();
  
  /* Error checking */
  /* Setup */
  /* Main */
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The GSSUnwrap object has the wrong GSS function",
    (void *)&gss_unwrap,
    (void *)cmd.getGSSFunction()
  );
  
  /* Cleanup */
  /* Return */
}

void GSSUnwrapTest::testEmptyCall()
{
  /* Variables */
  GSSUnwrap cmd = GSSUnwrap(&mock_unwrap);
  GSSBuffer input((char *)"Input message");
  gss_ctx_id_t desiredContext = (gss_ctx_id_t)( (long)0 | rand());
  
  /* Error checking */
  /* Setup */
  cmd.setContextHandle(desiredContext);
  cmd.setInputMessage(&input);
  
  MockUnwrap::minor_status = 0;
  MockUnwrap::retVal = 0;
  MockUnwrap::conf_state = rand();
  MockUnwrap::qop_state = GSS_C_QOP_DEFAULT;
  MockUnwrap::outputMessageBuffer.setValue((char *)"Output message");
  
  
  /* Main */
  cmd.execute();
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The requested GSS context handle is not correct",
    desiredContext,
    MockUnwrap::context_handle
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The input message was incorrect.",
    input.toString(),
    MockUnwrap::inputMessageBuffer.toString()
  );
  
  
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The conf_state flag was incorrect.",
    MockUnwrap::qop_state,
    cmd.getQopState()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The conf_state flag was incorrect.",
    MockUnwrap::conf_state,
    cmd.getConfState()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The input message was incorrect.",
    cmd.getOutputMessage().toString(),
    MockUnwrap::outputMessageBuffer.toString()
  );
  
  
  /* Cleanup */
  /* Return */
}

void GSSUnwrapTest::testConstructorWithJSONObject()
{
  /* Variables */
  GSSContext context((gss_ctx_id_t)( (long)0 | rand()),
		     true);
  std::string key = GSSContextCache::instance()->store(context);
  std::string input = "{ \
         \"context_handle\": \"" + key + "\", \
         \"input_message\": \"mary had a little lamb\" \
    }";
  json_error_t jsonErr;
  JSONObject json = JSONObject::load(input.c_str(), 0, &jsonErr);
  
  GSSUnwrap cmd = GSSUnwrap(&json, &mock_unwrap);
  
  /* Error checking */
  /* Setup */
  /* Main */
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSUnwrap did not retrieve the GSS context correctly",
    context.getContext(),
    cmd.getContextHandle()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSUnwrap did not parse the input message argument correctly",
    std::string("mary had a little lamb"),
    cmd.getInputMessage().toString()
  );
  
  
  
  /* Cleanup */
  /* Return */
}


void GSSUnwrapTest::testJSONMarshal()
{
  /* Variables */
  std::string output("dns@google.com");
  int confState = 1;
  gss_qop_t qopState = GSS_C_QOP_DEFAULT;
  JSONObject *result;
  GSSUnwrap cmd = GSSUnwrap(&mock_unwrap);
  
  /* Error checking */
  /* Setup */
  MockUnwrap::minor_status = 0;
  MockUnwrap::retVal = 0;
  MockUnwrap::outputMessageBuffer.setValue(output);
  MockUnwrap::conf_state = confState;
  MockUnwrap::qop_state = qopState;
  
  /* Main */
  cmd.execute();
  result = cmd.toJSON();
//   std::cout << "\nGSSUnwrap JSON: \n" << result->dump() << "\n";
  
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The return value was reported incorrectly",
    (int)MockUnwrap::retVal,
    (int)( (*result)["major_status"].integer() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The minor_status value was reported incorrectly",
    (int)MockUnwrap::minor_status,
    (int)( (*result)["minor_status"].integer() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The output message was reported incorrectly",
    MockUnwrap::outputMessageBuffer.toString(),
    std::string( (*result)["output_message"].string() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The minor_status value was reported incorrectly",
    (int)qopState,
    (int)( (*result)["qop_state"].integer() )
  );
  
  
  /* Cleanup */
  /* Return */
}
