# Copyright (c) 2014, 2015 JANET(UK)
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of JANET(UK) nor the names of its contributors
#    may be used to endorse or promote products derived from this software
#    without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
# COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
# INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
# HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
# STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
# OF THE POSSIBILITY OF SUCH DAMAGE.

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/commands)

find_package (PkgConfig)
pkg_check_modules (GLIB2   glib-2.0)
if (GLIB2_FOUND)
  include_directories(${GLIB2_INCLUDE_DIRS})
endif(GLIB2_FOUND)
  
if (WIN32)
else (WIN32)
add_executable(test GSSExceptionTest.cpp 
                    GSSAcquireCredTest.cpp 
                    GSSGetMicTest.cpp
                    GSSPseudoRandomTest.cpp 
                    GSSWrapTest.cpp 
                    GSSUnwrapTest.cpp 
                    NameCacheTest.cpp
                    command_mocks/InitSecContextMock.cpp
                    command_mocks/MockAcquireCred.cpp
                    command_mocks/MockGetMic.cpp
                    command_mocks/MockPseudoRandom.cpp 
                    command_mocks/MockUnwrap.cpp
                    command_mocks/MockWrap.cpp 
                    GSSCreateSecContextTest.cpp 
                    GSSImportNameTest.cpp
                    command_mocks/MockImportName.cpp
                    test_run.cpp 
                    ../src/commands/GSSInitSecContext.cpp 
                    ../src/utils/util_base64.cpp
                    ../src/utils/util_json.cpp 
                    ../src/utils/util_random.cpp
                    ../src/commands/GSSImportName.cpp
                    ../src/GSSException.cpp
                    ../src/commands/GSSGetMic.cpp
                    ../src/commands/GSSAcquireCred.cpp
                    ../src/commands/GSSPseudoRandom.cpp
                    ../src/commands/GSSUnwrap.cpp
                    ../src/commands/GSSWrap.cpp
                    ../src/datamodel/GSSBuffer.cpp
                    ../src/datamodel/GSSCredential.cpp
                    ../src/datamodel/GSSDisplayStatus.cpp
                    ../src/datamodel/GSSName.cpp
                    ../src/datamodel/GSSOID.cpp
                    ../src/datamodel/GSSOIDSet.cpp
                    ../src/datamodel/GSSContext.cpp
                    ../src/cache/GSSContextCache.cpp
                    ../src/cache/GSSNameCache.cpp
                    datamodel/GSSBufferTest.cpp
                    datamodel/GSSOIDSetTest.cpp
  )

find_package (Threads)

target_link_libraries(test cppunit gssapi_krb5 jansson crypto ${GLIB2_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})
# install(TARGETS test RUNTIME DESTINATION bin)
endif(WIN32)

add_subdirectory(datamodel)
add_subdirectory(command_mocks)
