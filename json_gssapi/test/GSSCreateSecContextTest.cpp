/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */


#include "GSSCreateSecContextTest.h"
#include "GSSCreateSecContextCommand.h"
#include "InitSecContextMock.h"
#include <iostream>
#include <string.h>
#include <exception>
#include "util_json.h"

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( GSSCreateSecContextTest );



static OM_uint32 KRB5_CALLCONV
mock_init_sec(
    OM_uint32             *minor_status,
    gss_cred_id_t          claimant_cred_handle,
    gss_ctx_id_t          *context_handle,
    gss_name_t             target_name,
    gss_OID                mech_type,
    OM_uint32              req_flags,
    OM_uint32              time_req,
    gss_channel_bindings_t input_chan_bindings,
    gss_buffer_t           input_token,
    gss_OID               *actual_mech_type,
    gss_buffer_t           output_token,
    OM_uint32             *ret_flags,
    OM_uint32             *time_rec)
{
  InitSecContextMock::visited = true;
  
  /* Copy in the input to this function */
  InitSecContextMock::claimant_cred_handle = claimant_cred_handle;
  InitSecContextMock::target_name = target_name;
  InitSecContextMock::mech_type = mech_type;
  InitSecContextMock::req_flags = req_flags;
  InitSecContextMock::time_req = time_req;
  InitSecContextMock::input_chan_bindings = input_chan_bindings;
  InitSecContextMock::input_token.length = input_token->length;
  InitSecContextMock::input_token.value = input_token->value;
  
  
  /* Copy out the output from this function */
  *minor_status = InitSecContextMock::minor_status;
  *actual_mech_type = InitSecContextMock::actual_mech_type;
  output_token->length = InitSecContextMock::output_token.length;
  output_token->value = InitSecContextMock::output_token.value;
  *ret_flags = InitSecContextMock::ret_flags;
  *time_rec = InitSecContextMock::time_rec;
  
  /* Handle the one that's I/O */
    if (*context_handle == GSS_C_NO_CONTEXT)
  {
    *context_handle = InitSecContextMock::context_handle;
  } else if (*context_handle != InitSecContextMock::context_handle)
  {
    InitSecContextMock::invalidContextHandle = true;
  }

  return InitSecContextMock::retVal;
}


void
GSSCreateSecContextTest::setUp()
{
  InitSecContextMock::reset();
}

void
GSSCreateSecContextTest::tearDown()
{
}

void
GSSCreateSecContextTest::testConstructor()
{
  GSSCreateSecContextCommand cmd = GSSCreateSecContextCommand();
  void *cmdFn;
  void *GSSFn;
  
  cmdFn = cmd.getGSSFunction();
  GSSFn = (void *)&gss_init_sec_context;
  CPPUNIT_ASSERT_MESSAGE("The default constructor for GSSCreateSecContextCommand should assign the function gss_init_sec_context", cmdFn == GSSFn);
}


void GSSCreateSecContextTest::testConstructorWithJSONObject()
{
  const char* input = "{\"method\": \"gss_create_sec_context\", \
    \"arguments\": {\"req_flags\": \"1\", \
    \"time_req\": \"2\", \
    \"mech_type\": \"{ 1 2 840 113554 1 2 1 4 }\", \
    \"target_name\": \"me@my.sha/DOW\"}}";

  json_error_t jsonErr;
  JSONObject json = JSONObject::load(input, 0, &jsonErr);
  
  GSSCreateSecContextCommand cmd = GSSCreateSecContextCommand(
    &json, 
    (void *)&mock_init_sec
  );
  
  
  const char *from_json = json["arguments"]["target_name"].string();
  const char *from_cmd = cmd.getTargetDisplayName();
  
  CPPUNIT_ASSERT_MESSAGE(
    "The object does not have a target name.",
    ( strcmp(from_json, from_cmd) == 0 )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The context_handle values differ.",
    json["arguments"]["context_handle"].integer(),
    (json_int_t)cmd.getContextHandle()
  );
  
  CPPUNIT_ASSERT_MESSAGE(
    "The mech_type values differ.",
    ( strcmp(json["arguments"]["mech_type"].string(), cmd.getMechType()) == 0 )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The req_flags differ.",
    (int)json["arguments"]["req_flags"].integer(),
    (int)cmd.getReqFlags()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The req_flags differ.",
    (int)json["arguments"]["time_req"].integer(),
    (int)cmd.getTimeReq()
  );
  
}

void
GSSCreateSecContextTest::testEmptyCall()
{
  GSSCreateSecContextCommand cmd ((void *)&mock_init_sec);
  
  /* Set expectations on what the GSS function will be called with */
  cmd.time_req = rand() % 1024;
  cmd.req_flags = rand() % 1024;
  cmd.target_name = NULL;
  cmd.context_handle = GSS_C_NO_CONTEXT;
  
  CPPUNIT_ASSERT_MESSAGE(
    "The mech_type values differ.",
    ( strcmp("{ 1 2 840 113554 1 2 1 4 }", cmd.getMechType()) == 0 )
  );
  
  
  
  /* Set expectations on what the GSS function will produce */
  InitSecContextMock::retVal = rand() % 1024;
  InitSecContextMock::minor_status = rand() % 1024;
  InitSecContextMock::context_handle = GSS_C_NO_CONTEXT;
  InitSecContextMock::actual_mech_type = NULL;
  InitSecContextMock::output_token.value = (void *)"http@project-moonshot.org/PROJECT-MOONSHOT.ORG\0";
  InitSecContextMock::output_token.length = strlen((char *)InitSecContextMock::output_token.value);
  InitSecContextMock::ret_flags = rand() % 1024;
  InitSecContextMock::time_req = rand() % 1024;
  
  cmd.execute();
  
  /* Check that init_sec_context's inputs are sent correctly */
  CPPUNIT_ASSERT_MESSAGE(
    "The GSS function was not invoked!",
    InitSecContextMock::visited
  );
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The time_req field was not used in the call to init_sec_context",
    cmd.time_req,
    InitSecContextMock::time_req
  );
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The req_flags field was not used in the call to init_sec_context",
    cmd.req_flags,
    InitSecContextMock::req_flags
  );
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The mech_type field was not used in the call to init_sec_context",
    cmd.mech_type,
    InitSecContextMock::mech_type
  );
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The target_name field was not used in the call to init_sec_context",
    cmd.target_name,
    InitSecContextMock::target_name
  );
  
  
  /* Check that init_sec_context's outputs are captured correctly */
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "Return value was not copied back to the command.",
    InitSecContextMock::retVal,
    cmd.retVal
  );
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "Status was not copied back to the command.",
    InitSecContextMock::minor_status,
    cmd.minor_status
  );
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "context_handle was not copied back to the command.",
    InitSecContextMock::context_handle,
    cmd.context_handle
  );
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "actual_mech_type was not copied back to the command.",
    InitSecContextMock::actual_mech_type,
    cmd.actual_mech_type
  );
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "output_token was not copied back to the command.",
    InitSecContextMock::output_token.value,
    cmd.output_token.value
  );
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "ret_flags was not copied back to the command.",
    InitSecContextMock::ret_flags,
    cmd.ret_flags
  );
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "time_rec was not copied back to the command.",
    InitSecContextMock::time_rec,
    cmd.time_rec
  );
}

void GSSCreateSecContextTest::testJSONMarshal()
{
  /* Variables */
  GSSCreateSecContextCommand cmd ((void *)&mock_init_sec);
  JSONObject *result;
  
  /* Error checking */
  
  /* Setup */
  // Set expectations on what the GSS function will produce
  InitSecContextMock::retVal = GSS_S_BAD_MECH;
  InitSecContextMock::minor_status = 20;
  InitSecContextMock::context_handle = GSS_C_NO_CONTEXT;
  InitSecContextMock::actual_mech_type = (gss_OID)GSS_C_MA_MECH_NEGO;
  InitSecContextMock::output_token.value = (void *)"http@project-moonshot.org/PROJECT-MOONSHOT.ORG\0";
  InitSecContextMock::output_token.length = strlen((char *)InitSecContextMock::output_token.value);
  InitSecContextMock::ret_flags = GSS_C_MUTUAL_FLAG | 
      GSS_C_REPLAY_FLAG | 
      GSS_C_SEQUENCE_FLAG | 
      GSS_C_CONF_FLAG | 
      GSS_C_INTEG_FLAG | 
      GSS_C_ANON_FLAG | 
      GSS_C_PROT_READY_FLAG;
  InitSecContextMock::time_req = GSS_C_INDEFINITE;
  
  /* Main */
  cmd.execute();
  result = cmd.toJSON();
  
  CPPUNIT_ASSERT_MESSAGE(
    "The command name is incorrect",
    ( strcmp("gss_init_sec_context", 
	     (*result)["command"].string() ) == 0 )
  );
  
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The return value was reported incorrectly",
    (int)InitSecContextMock::retVal,
    (int)( (*result)["return_values"]["major_status"].integer() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The minor_status value was reported incorrectly",
    (int)InitSecContextMock::minor_status,
    (int)( (*result)["return_values"]["minor_status"].integer() )
  );
  
  CPPUNIT_ASSERT_MESSAGE(
    "The actual_mech_type value was reported incorrectly",
    ( strcmp("{ 1 3 6 1 5 5 13 4 }", 
	     (*result)["return_values"]["actual_mech_type"].string() ) == 0 )
  );
  
  CPPUNIT_ASSERT_MESSAGE(
    "The output_token value was reported incorrectly",
    ( strcmp((const char *)(InitSecContextMock::output_token.value), 
	     (*result)["return_values"]["output_token"].string() ) == 0 )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The minor_status value was reported incorrectly",
    (int)InitSecContextMock::ret_flags,
    (int)( (*result)["return_values"]["ret_flags"].integer() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The minor_status value was reported incorrectly",
    (int)InitSecContextMock::time_rec,
    (int)( (*result)["return_values"]["time_rec"].integer() )
  );
  
  
  /* Cleanup */
  /* Return */
}


