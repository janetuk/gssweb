/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "GSSGetMicTest.h"
#include "command_mocks/MockGetMic.h"
#include "GSSGetMic.h"
#include <datamodel/GSSContext.h>
#include <cache/GSSContextCache.h>
#include <gssapi/gssapi.h>

CPPUNIT_TEST_SUITE_REGISTRATION( GSSGetMicTest );

/* 
 * a mock of the gss_import_name call
 * 
 * Basically, just copy the arguments over to/from the
 * MockGetMic global object
 */

static OM_uint32 KRB5_CALLCONV
mock_get_mic(
    OM_uint32    *minor_status,
    gss_ctx_id_t  context_handle,
    gss_qop_t     qop_req,
    gss_buffer_t  message_buffer,
    gss_buffer_t  message_token)
{
  /* Error checking */
  /* Variables */
  
  /* Setup */
  
  /* Main */
  // Copy our input from the appropriate parameters to MockGetMic
  MockGetMic::context_handle = context_handle;
  MockGetMic::qop_req        = qop_req;
  MockGetMic::inputMessage.setValue((char *)message_buffer->value, message_buffer->length);
  
  
  // copy our output to the appropriate parameters
  *minor_status = MockGetMic::minor_status;
  *message_token = *MockGetMic::outputToken.toGss();
  
  /* Cleanup */
  /* return */
  return MockGetMic::retVal;
}

void GSSGetMicTest::setUp()
{
  CppUnit::TestFixture::setUp();
  MockGetMic::reset();
}

void GSSGetMicTest::testConstructor()
{
  /* Variables */
  GSSGetMic cmd = GSSGetMic();
  
  /* Error checking */
  /* Setup */
  /* Main */
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The GSSGetMic object has the wrong GSS function",
    (void *)&gss_get_mic,
    (void *)cmd.getGSSFunction()
  );
  
  /* Cleanup */
  /* Return */
}

void GSSGetMicTest::testEmptyCall()
{
  /* Variables */
  GSSGetMic cmd = GSSGetMic(&mock_get_mic);
  GSSBuffer input((char *)"Input message");
  std::string out("Output Message");
  gss_qop_t desiredQop = rand();
  gss_ctx_id_t desiredContext = (gss_ctx_id_t)rand();
  
  /* Error checking */
  /* Setup */
  cmd.setContextHandle(desiredContext);
  cmd.setQopReq( desiredQop );
  cmd.setInputMessage(&input);
  
  MockGetMic::minor_status = 0;
  MockGetMic::retVal = 0;
  MockGetMic::outputToken.setValue(out);
  
  
  /* Main */
  cmd.execute();
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The requested GSS context handle is not correct",
    desiredContext,
    MockGetMic::context_handle
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The qop_req flag was incorrect.",
    desiredQop,
    MockGetMic::qop_req
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The input message was incorrect.",
    input.toString(),
    MockGetMic::inputMessage.toString()
  );
  
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The output message token was incorrect.",
    out,
    cmd.getOutputToken().toString()
  );
  
  
  /* Cleanup */
  /* Return */
}

void GSSGetMicTest::testConstructorWithJSONObject()
{
  /* Variables */
  GSSContext context((gss_ctx_id_t)rand(), true);
  std::string key = GSSContextCache::instance()->store(context);

  std::string input = "{ \
         \"context_handle\": \"" + key + "\", \
         \"qop_req\": \"GSS_C_QOP_DEFAULT\", \
         \"input_message\": \"mary had a little lamb\" \
    }";
  json_error_t jsonErr;
  JSONObject json = JSONObject::load(input.c_str(), 0, &jsonErr);
  
  GSSGetMic cmd = GSSGetMic(&json, &mock_get_mic);
  
  /* Error checking */
  /* Setup */
  /* Main */
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSGetMic did not parse the qop_req argument correctly",
    (gss_qop_t)0,
    cmd.getQopReq()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSGetMic did not parse the input message argument correctly",
    std::string("mary had a little lamb"),
    cmd.getInputMessage().toString()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSGetMic did not retrieve the context handle correctly",
    context.getContext(),
    cmd.getContextHandle()
  );
  
  /* Cleanup */
  /* Return */
}


/* Desired JSON output:
 * 
 * {
 *  "command":       "gss_get_mic",
 *  "return_values": 
 *  {
 *      "major_status": 0,
 *      "minor_status": 0,
 *      "output_token": "asdf"
 *  }
 * }
 */

void GSSGetMicTest::testJSONMarshal()
{
  /* Variables */
  std::string output("dns@google.com");
  GSSBuffer input( (char*)"abc" );
  JSONObject *result;
  GSSGetMic cmd = GSSGetMic(&mock_get_mic);
  
  /* Error checking */
  /* Setup */
  MockGetMic::minor_status = 0;
  MockGetMic::retVal = 0;
  MockGetMic::outputToken.setValue(output);
  
  cmd.setContextHandle(GSS_C_NO_CONTEXT);
  cmd.setInputMessage( &input );
  cmd.setQopReq(GSS_C_QOP_DEFAULT);
  
  /* Main */
  cmd.execute();
  result = cmd.toJSON();
//   std::cout << "\nGSSGetMic JSON: \n" << result->dump() << "\n";
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The return value was reported incorrectly",
    (int)MockGetMic::retVal,
    (int)( (*result)["major_status"].integer() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The minor_status value was reported incorrectly",
    (int)MockGetMic::minor_status,
    (int)( (*result)["minor_status"].integer() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The output message was reported incorrectly",
    output,
    std::string( (*result)["output_token"].string() )
  );
  
  
  /* Cleanup */
  /* Return */
}
