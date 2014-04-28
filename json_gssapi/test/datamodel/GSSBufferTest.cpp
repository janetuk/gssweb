/*
 * Copyright (c) 2014 <copyright holder> <email>
 *
 * For license details, see the LICENSE file in the root of this project.
 *
 */

#include "GSSBufferTest.h"
#include <datamodel/GSSBuffer.h>
#include <cstring>

CPPUNIT_TEST_SUITE_REGISTRATION( GSSBufferTest );

void
GSSBufferTest::setUp()
{

}

void
GSSBufferTest::tearDown()
{

}

void GSSBufferTest::testStringConstructor()
{
    /* Variables */
    std::string str("test string");
    GSSBuffer buf(str);
    gss_buffer_t gssBuf = buf.toGss();
    
    /* Error checking */
    /* Setup */
    
    /* Main */
    CPPUNIT_ASSERT_MESSAGE(
        "The string was not copied in :(", 
        (std::strncmp( (char *)str.c_str(), (char *)gssBuf->value, gssBuf->length ) == 0)
    );
    
    CPPUNIT_ASSERT_EQUAL_MESSAGE(
        "The string was not output correctly",
	str,
	buf.toString()
    );
    
    /* Cleanup */
    /* Return */
}
