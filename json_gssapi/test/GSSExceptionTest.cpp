/*
 * Copyright (c) 2014 <copyright holder> <email>
 * 
 * For license details, see the LICENSE file in the root of this project.
 * 
 */

#include "GSSExceptionTest.h"
#include "GSSException.h"
#include <string>
#include <iostream>


// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( GSSExceptionTest );

void GSSExceptionTest::setUp()
{
  CppUnit::TestFixture::setUp();
}

void GSSExceptionTest::tearDown()
{
  CppUnit::TestFixture::tearDown();
}


void GSSExceptionTest::testWithMessage()
{
  try {
    throw GSSException("Test message");
  } catch (GSSException e)
  {
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
      "GSSException message reporting is incorrect.",
      std::string("Test message\n\
GSS Error message:\n\
  Major status:\n\
    The routine completed successfully\n\
\n\
  Minor status details: \n\
    Unknown error\n"), 
      std::string(e.what())
    );
  }
}

void GSSExceptionTest::testWithMessageAndMajor()
{
  try {
    throw GSSException("Test message", 524288);
  } catch (GSSException e)
  {
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
      "GSSException message reporting is incorrect.",
      std::string("Test message\n\
GSS Error message:\n\
  Major status:\n\
    No context has been established\n\
\n\
  Minor status details: \n\
    Unknown error\n"), 
      std::string(e.what())
    );
  }
}

void GSSExceptionTest::testWithMessageAndMajorAndMinor()
{
  OM_uint32 major, minor, context = 0;
  gss_buffer_desc_struct buf;
  
  try {
    major = gss_display_status(&minor, 2109382930, GSS_C_MECH_CODE, GSS_C_NO_OID, &context, &buf);
    throw GSSException("Test message", major, minor);
  } catch (GSSException e)
  {
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
      "GSSException message reporting is incorrect.",
      std::string("Test message\n\
GSS Error message:\n\
  Major status:\n\
    An invalid status code was supplied\n\
\n\
  Minor status details: \n\
    Invalid argument\n"), 
      std::string(e.what())
    );
  }
}

void GSSExceptionTest::testWithMessageAndMajorAndMinorAndMechanism()
{

  OM_uint32 major, minor, context = 0;
  gss_buffer_desc_struct buf;
  
  try {
    major = gss_display_status(&minor, 2109382930, GSS_C_MECH_CODE, GSS_C_NO_OID, &context, &buf);
    throw GSSException("Test message", major, minor, GSS_C_NO_OID);
  } catch (GSSException e)
  {
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
      "GSSException message reporting is incorrect.",
      std::string("Test message\n\
GSS Error message:\n\
  Major status:\n\
    An invalid status code was supplied\n\
\n\
  Minor status details: \n\
    Invalid argument\n"), 
      std::string(e.what())
    );
  }
  
}