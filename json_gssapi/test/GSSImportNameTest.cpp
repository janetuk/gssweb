/*
 * Copyright (c) 2014 Painless Security LLC
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
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

#include "GSSImportNameTest.h"
#include "GSSImportName.h"
#include "MockImportName.h"

CPPUNIT_TEST_SUITE_REGISTRATION( GSSImportNameTest );


/* 
 * a mock of the gss_import_name call
 * 
 * Basically, just copy the arguments over to/from the
 * MockImportName global object
 */
static OM_uint32 KRB5_CALLCONV
mock_import_name(
    OM_uint32     *minor_status,
    gss_buffer_t   input_name_buffer, 
    gss_OID        input_name_type,
    gss_name_t    *output_name)
{
  /* Error checking */
  /* Variables */
  
  /* Setup */
  /* Main */
  // Copy our input from the appropriate parameters to MockImportName
  MockImportName::input_name_buffer.setValue((char *)input_name_buffer->value, input_name_buffer->length);
  MockImportName::input_name_type.setValue(input_name_type);
  
  // copy our output to the appropriate parameters
  *minor_status = MockImportName::minor_status;
  *output_name = MockImportName::output_name.toGSS();
  
  
  /* Cleanup */
  /* return */
  return MockImportName::retVal;
}

void GSSImportNameTest::setUp()
{
  CppUnit::TestFixture::setUp();
  MockImportName::reset();
}

void GSSImportNameTest::tearDown()
{
    CppUnit::TestFixture::tearDown();
}

void GSSImportNameTest::testConstructor()
{
  /* Variables */
  GSSImportName cmd = GSSImportName();
  
  /* Error checking */
  /* Setup */
  /* Main */
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The GSSImportName object has the wrong GSS function",
    (void *)&gss_import_name,
    (void *)cmd.getGSSFunction()
  );
  
  /* Cleanup */
  /* Return */
}
void GSSImportNameTest::testEmptyCall()
{
  /* Variables */
  GSSImportName cmd = GSSImportName(&mock_import_name);
  std::string name = std::string("ssh@server");
  std::string type = std::string("{ 1 2 840 113554 1 2 1 4 }");
  
  /* Error checking */
  /* Setup */
  cmd.setInputName(name);
  cmd.setInputNameType(type);
  MockImportName::minor_status = rand() % 1024;
  MockImportName::retVal = rand() % 1024;
  MockImportName::output_name.setValue(GSS_C_NO_NAME);
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "Input name was not set correctly.",
    name,
    cmd.getInputName().toString()
  );
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "Input name was not set correctly.",
    type,
    cmd.getInputNameType().toString()
  );
  
  cmd.execute();
  
  /* Main */
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The requested GSS name is not correct",
    name,
    MockImportName::input_name_buffer.toString()
  );
  
  GSSOID retOID = GSSOID(MockImportName::input_name_type);
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "The requested GSS name type is not correct",
    type,
    retOID.toString()
  );
  
  /* Cleanup */
  /* Return */
}
void GSSImportNameTest::testConstructorWithJSONObject()
{
  /* Variables */
  const char* input = "{\"method\": \"gss_import_name\", \
    \"arguments\": {\"input_name\": \"http@localhost\", \
    \"input_name_type\": \"{ 1 2 840 113554 1 2 1 4 }\"}}";
  json_error_t jsonErr;
  JSONObject json = JSONObject::load(input, 0, &jsonErr);
  
  GSSImportName cmd = GSSImportName(&json, &mock_import_name);
  
  /* Error checking */
  /* Setup */
  /* Main */
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSImportName did not parse the input_name argument correctly",
    std::string("http@localhost"),
    cmd.getInputName().toString()
  );
  
  CPPUNIT_ASSERT_EQUAL_MESSAGE(
    "GSSImportName did not parse the input_name_type argument correctly",
    std::string("{ 1 2 840 113554 1 2 1 4 }"),
    cmd.getInputNameType().toString()
  );
  
  /* Cleanup */
  /* Return */
}
void GSSImportNameTest::testJSONMarshal()
{
  /* Variables */
  GSSImportName cmd = GSSImportName();
  
  /* Error checking */
  /* Setup */
  /* Main */
  
  /* Cleanup */
  /* Return */
}

