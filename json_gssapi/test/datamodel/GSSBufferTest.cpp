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
