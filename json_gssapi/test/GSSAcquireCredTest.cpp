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
#include <gssapi.h>

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

}

void GSSAcquireCredTest::testEmptyCall()
{
  /* Variables */
  GSSAcquireCred cmd = GSSAcquireCred(&mock_acquire_cred);
  OM_uint32 minor;
  GSSName steve((char *)"steve@local", (gss_OID)GSS_C_NT_USER_NAME);
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

void GSSAcquireCredTest::testJSONMarshal()
{

}
