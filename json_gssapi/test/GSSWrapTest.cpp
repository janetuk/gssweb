/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "GSSWrapTest.h"
#include "command_mocks/MockWrap.h"
#include "GSSWrap.h"
#include <datamodel/GSSContext.h>
#include <cache/GSSContextCache.h>
#include <gssapi/gssapi.h>

CPPUNIT_TEST_SUITE_REGISTRATION( GSSWrapTest );

/* 
 * a mock of the gss_import_name call
 * 
 * Basically, just copy the arguments over to/from the
 * MockWrap global object
 */

static OM_uint32 KRB5_CALLCONV
mock_wrap(
    OM_uint32    *minor_status,
    gss_ctx_id_t  context_handle,
    int           conf_req_flag,
    gss_qop_t     qop_req,
    gss_buffer_t  input_message_buffer,
    int          *conf_state,
    gss_buffer_t  output_message_buffer)
{
  /* Error checking */
  /* Variables */
  
  /* Setup */
  
  /* Main */
  // Copy our input from the appropriate parameters to MockWrap
  MockWrap::context_handle = context_handle;
  MockWrap::conf_req_flag  = conf_req_flag;
  MockWrap::qop_req        = qop_req;
  MockWrap::inputMessageBuffer.setValue(input_message_buffer);
  
  
  // copy our output to the appropriate parameters
  *minor_status = MockWrap::minor_status;
  *conf_state = MockWrap::conf_state;
  *output_message_buffer = *MockWrap::outputMessageBuffer.toGss();
  
  /* Cleanup */
  /* return */
  return MockWrap::retVal;
}

void GSSWrapTest::setUp()
{
  CppUnit::TestFixture::setUp();
  MockWrap::reset();
}

void GSSWrapTest::testConstructor()
{
  /* Variables */
  GSSWrap cmd = GSSWrap();
  
  /* Error checking */
  /* Setup */
  /* Main */
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The GSSWrap object has the wrong GSS function",
    (void *)&gss_wrap,
    (void *)cmd.getGSSFunction()
  );
  
  /* Cleanup */
  /* Return */
}

void GSSWrapTest::testEmptyCall()
{
  /* Variables */
  GSSWrap cmd = GSSWrap(&mock_wrap);
  GSSBuffer input((char *)"Input message");
  gss_qop_t desiredQop = rand();
  int desiredConf = rand();
  gss_ctx_id_t desiredContext = (gss_ctx_id_t)( (long)0 | rand() );
  
  /* Error checking */
  /* Setup */
  cmd.setContextHandle(desiredContext);
  cmd.setConfReq( desiredConf );
  cmd.setQopReq( desiredQop );
  cmd.setInputMessage(&input);
  
  MockWrap::minor_status = 0;
  MockWrap::retVal = 0;
  MockWrap::conf_state = rand();
  MockWrap::outputMessageBuffer.setValue((char *)"Output message");
  
  
  /* Main */
  cmd.execute();
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The requested GSS context handle is not correct",
    desiredContext,
    MockWrap::context_handle
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The conf_req flag was incorrect.",
    desiredConf,
    MockWrap::conf_req_flag
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The qop_req flag was incorrect.",
    desiredQop,
    MockWrap::qop_req
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The input message was incorrect.",
    input.toString(),
    MockWrap::inputMessageBuffer.toString()
  );
  
  
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The conf_state flag was incorrect.",
    MockWrap::conf_state,
    cmd.getConfState()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The input message was incorrect.",
    cmd.getOutputMessage().toString(),
    MockWrap::outputMessageBuffer.toString()
  );
  
  
  /* Cleanup */
  /* Return */
}

/*
 * Example JSON input:
 * {
 *   "method": "gss_wrap",
 *   "arguments": {
 *     "context_handle": "stuff",
 *     "conf_req": "TRUE",
 *     "qop_req": "GSS_C_QOP_DEFAULT",
 *     "input_message": "mary had a little lamb"
 *   }
 * }
 * 
 */
void GSSWrapTest::testConstructorWithJSONObject()
{
  /* Variables */
  GSSContext context( (gss_ctx_id_t)( (long)0 | rand()),
		      true );
  std::string key = GSSContextCache::instance()->store(context);
  
  std::string input = "{ \
         \"context_handle\": \"" + key + "\", \
         \"conf_req\": \"TRUE\", \
         \"qop_req\": \"GSS_C_QOP_DEFAULT\", \
         \"input_message\": \"mary had a little lamb\" \
    }";
  json_error_t jsonErr;
  JSONObject json = JSONObject::load(input.c_str(), 0, &jsonErr);
  
  GSSWrap cmd = GSSWrap(&json, &mock_wrap);
  
  /* Error checking */
  /* Setup */
  /* Main */
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSWrap did not retrive the GSS context correctly",
    context.getContext(),
    cmd.getContext()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSWrap did not parse the conf_req argument correctly",
    1,
    cmd.getConfReq()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSWrap did not parse the qop_req argument correctly",
    (gss_qop_t)0,
    cmd.getQopReq()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSWrap did not parse the input message argument correctly",
    std::string("mary had a little lamb"),
    cmd.getInputMessage().toString()
  );
  
  
  
  /* Cleanup */
  /* Return */
}

/* Desired JSON output:
 * 
 * {
 *  "command":       "gss_wrap",
 *  "return_values": 
 *  {
 *      "major_status":   0,
 *      "minor_status":   0,
 *      "conf_state":     "TRUE",
 *      "output_message": "asdf"
 *  }
 * }
 */
void GSSWrapTest::testJSONMarshal()
{
  /* Variables */
  std::string output("dns@google.com");
  int confState = 1;
  JSONObject *result;
  GSSWrap cmd = GSSWrap(&mock_wrap);
  
  /* Error checking */
  /* Setup */
  MockWrap::minor_status = 0;
  MockWrap::retVal = 0;
  MockWrap::outputMessageBuffer.setValue(output);
  MockWrap::conf_state = confState;
  
  /* Main */
  cmd.execute();
  result = cmd.toJSON();
//   std::cout << "\nGSSWrap JSON: \n" << result->dump() << "\n";
  
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The return value was reported incorrectly",
    (int)MockWrap::retVal,
    (int)( (*result)["major_status"].integer() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The minor_status value was reported incorrectly",
    (int)MockWrap::minor_status,
    (int)( (*result)["minor_status"].integer() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The output message was reported incorrectly",
    MockWrap::outputMessageBuffer.toString(),
    std::string( (*result)["output_message"].string() )
  );
  
  
  /* Cleanup */
  /* Return */
}
