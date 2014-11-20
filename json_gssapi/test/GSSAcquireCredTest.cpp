/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "GSSAcquireCredTest.h"
#include "GSSAcquireCred.h"
#include "command_mocks/MockAcquireCred.h"
#include <datamodel/GSSName.h>
#include <cache/GSSNameCache.h>
#include <gssapi.h>
#include <stdexcept>

CPPUNIT_TEST_SUITE_REGISTRATION( GSSAcquireCredTest );

OM_uint32 KRB5_CALLCONV
mock_acquire_cred(
    OM_uint32 *minor_status,
    gss_name_t desired_name,
    OM_uint32 time_req,
    gss_OID_set desired_mechs,
    gss_cred_usage_t cred_usage,
    gss_cred_id_t * output_cred_handle,
    gss_OID_set * actual_mechs,
    OM_uint32 *time_rec)
{
  
  // Set MockAcquireCred attributes from our in-parameters
  MockAcquireCred::desired_name = desired_name;
  MockAcquireCred::time_req = time_req;
  MockAcquireCred::desired_mechs = desired_mechs;
  MockAcquireCred::cred_usage = cred_usage;
  
  // Set our out-parameters from MockAcquireCred
  *output_cred_handle = MockAcquireCred::output_cred_handle;
  *actual_mechs = MockAcquireCred::actual_mechs;
  *time_rec = MockAcquireCred::time_rec;
  
  *minor_status = MockAcquireCred::minor_status;
  return(MockAcquireCred::retVal);
}

void GSSAcquireCredTest::setUp()
{

}

void GSSAcquireCredTest::tearDown()
{

}

void GSSAcquireCredTest::testConstructor()
{
  /* Variables */
  GSSAcquireCred cmd = GSSAcquireCred();
  
  /* Error checking */
  /* Setup */
  /* Main */
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The GSSImportName object has the wrong GSS function",
    &gss_acquire_cred,
    cmd.getGSSFunction()
  );
  
  /* Cleanup */
  /* Return */
}

void GSSAcquireCredTest::testConstructorWithJSONObject()
{
  /* Variables */
  // To feed into the JSON
  OM_uint32 time_req = rand();
  gss_cred_usage_t cred_usage = 2;
  GSSOID mech( (char *)"{ 1 2 840 113554 1 2 1 4 }" );
  std::stringstream input;
  char *desired_name = (char *)"HTTP@localhost\0";
  GSSName desired;
  OM_uint32 major, minor;
  gss_name_t des;
  
  
  major = gss_import_name(&minor, GSSBuffer(desired_name).toGss(), GSS_C_NT_HOSTBASED_SERVICE, &des);
  if (GSS_ERROR(major))
  {
    OM_uint32 min, context;
    gss_buffer_desc buf;
    
    std::cout << "Error in importing name." << std::endl;
    gss_display_status(&min, major, GSS_C_GSS_CODE, GSS_C_NT_HOSTBASED_SERVICE, &context, &buf);
    std::cout << "  message: " << (char *)buf.value << std::endl;
  }
  CPPUNIT_ASSERT_MESSAGE(
    "Could not generate a name to test GSSCreateSecContext JSON parsing.",
    !GSS_ERROR(major)
  );
  desired.setValue(des);
  std::string key = GSSNameCache::instance()->store(desired);

  // The JSON string
  input << 
  "{\"desired_name\" : \"" << key << "\", \
    \"time_req\"     : " << time_req << ", \
    \"cred_usage\"   : " << cred_usage << ", \
    \"desired_mechs\": [\"" << mech.toString() << "\"] \
   }";
  
  // The JSON itself
  json_error_t jsonErr;
  JSONObject json = JSONObject::load(input.str().c_str(), 0, &jsonErr);
  
  GSSOIDSet desiredMechs;
  
  
  GSSAcquireCred cmd = GSSAcquireCred(&json, &mock_acquire_cred);
  
  /* Error checking */
  /* Setup */
  /* Main */
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSAcquireCred's time_req was not loaded correctly",
    time_req,
    cmd.getTimeReq()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSAcquireCred's cred_usage was not loaded correctly",
    cred_usage,
    cmd.getCredUsage()
  );
  
  json.set( "cred_usage", "GSS_C_INITIATE" );
  cmd.loadParameters(&json);
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSAcquireCred's cred_usage was not loaded correctly",
    GSS_C_INITIATE,
    (int)cmd.getCredUsage()
  );
  
  // CPPUNIT_ASSERT_THROW(expression, exception);
  json.set( "cred_usage", "GSS_C_INITIATOR" );
  CPPUNIT_ASSERT_THROW_MESSAGE(
    "GSSAcquireCred's JSON parsing is admitting invalid strings.", 
    cmd.loadParameters(&json), 
    std::invalid_argument 
  );
  
  desiredMechs = cmd.getDesiredMechs();
  CPPUNIT_ASSERT_MESSAGE(
    "The desired mechanisms were not set properly.",
    desiredMechs.includes(mech)
  );
  
  /* Cleanup */
  /* Return */
}

void GSSAcquireCredTest::testEmptyCall()
{
  /* Variables */
  GSSAcquireCred cmd = GSSAcquireCred(&mock_acquire_cred);
  OM_uint32 minor;
  GSSName steve; // ((char *)"steve@local", (gss_OID)GSS_C_NT_USER_NAME);
  GSSOID moonshotOID((char *)"{1 3 6 1 5 5 15 1 1 18}");
  
  /* Error checking */
  /* Setup */
  cmd.setDesiredName(steve);
  cmd.setTimeReq(0);
  cmd.addDesiredMech( moonshotOID );
  cmd.setCredUsage(GSS_C_INITIATE);
  
  
  MockAcquireCred::retVal = 0;
  MockAcquireCred::minor_status = 0;
  MockAcquireCred::output_cred_handle = GSS_C_NO_CREDENTIAL;
  gss_create_empty_oid_set(&minor, &(MockAcquireCred::actual_mechs));
  MockAcquireCred::time_rec = rand();
  
  cmd.execute();
  
  /* Main */
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The desired name was not passed in correctly",
    steve.toGss(),
    MockAcquireCred::desired_name
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The time_req was not passed in correctly",
    cmd.getTimeReq(),
    MockAcquireCred::time_req
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The desired mechs were not passed in correctly",
    cmd.getDesiredMechs(),
    MockAcquireCred::desired_mechs
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The cred_usage was not passed in correctly",
    cmd.getCredUsage(),
    MockAcquireCred::cred_usage
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The output credential handle was not passed in correctly",
    MockAcquireCred::output_cred_handle,
    cmd.getOutputCredHandle()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The actual mechanisms value was not passed in correctly",
    MockAcquireCred::actual_mechs->count,
    cmd.getActualMechs()->count
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The time_rec value was not passed in correctly",
    MockAcquireCred::time_rec,
    cmd.getTimeRec()
  );
  
  /* Cleanup */
  /* Return */
}


/* Desired JSON output:
 * 
 * {
 *   "command": "gss_acquire_cred",
 *   "return_values": {
 *     "major_status": 0,
 *     "minor_status": 0,
 *     "cred_handle": "###########",
 *     "actual_mechs": [
 *       "{ 1 2 3 4 }",
 *       "{ 1 5 6 7 8 }"
 *     ],
 *     "time_rec": 0
 *   }
 * }
 */
void GSSAcquireCredTest::testJSONMarshal()
{
  /* Variables */
  GSSOIDSet actualMechs;
  JSONObject *result;
  GSSAcquireCred cmd = GSSAcquireCred(&mock_acquire_cred);
  
  /* Error checking */
  /* Setup */
  actualMechs.addOID( GSSOID( (char *)"{ 1 2 3 4 }" ) );
  actualMechs.addOID( GSSOID( (char *)"{ 1 5 6 7 8 }" ) );
  MockAcquireCred::reset();
  MockAcquireCred::retVal = 0;
  MockAcquireCred::minor_status = 0;
  MockAcquireCred::output_cred_handle = GSS_C_NO_CREDENTIAL;
  MockAcquireCred::actual_mechs = actualMechs.toGss();
  MockAcquireCred::time_rec = 0;
  
  /* Main */
  cmd.execute();
  result = cmd.toJSON();
  
//   std::cout << "\n" << result->dump() << "\n";
  
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The return value was reported incorrectly",
    (int)MockAcquireCred::retVal,
    (int)( (*result)["major_status"].integer() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The minor_status value was reported incorrectly",
    (int)MockAcquireCred::minor_status,
    (int)( (*result)["minor_status"].integer() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The gss_name was reported incorrectly",
    std::string("{ 1 2 3 4 }"),
    std::string( (*result)["actual_mechs"][(size_t)0].string() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The gss_name was reported incorrectly",
    std::string("{ 1 5 6 7 8 }"),
    std::string( (*result)["actual_mechs"][(size_t)1].string() )
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The minor_status value was reported incorrectly",
    (int)MockAcquireCred::time_rec,
    (int)( (*result)["time_rec"].integer() )
  );
  
  
  /* Cleanup */
  /* Return */
}

