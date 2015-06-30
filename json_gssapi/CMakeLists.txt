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

cmake_minimum_required(VERSION 2.8)
include(GenerateExportHeader)
include(GNUInstallDirs)
project(json_gssapi)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/ ${CMAKE_CURRENT_BINARY_DIR})

add_library(jsongssapi SHARED 
                       src/datamodel/GSSDisplayStatus.cpp 
                       src/datamodel/GSSName.cpp
                       src/datamodel/GSSOID.cpp
                       src/datamodel/GSSOIDSet.cpp
                       src/datamodel/GSSBuffer.cpp
                       src/datamodel/GSSCredential.cpp
                       src/GSSException.cpp
                       src/GSSRequest.cpp 
                       src/commands/GSSGetMic.cpp
                       src/commands/GSSInitSecContext.cpp
                       src/commands/GSSCommand.cpp 
                       src/commands/GSSImportName.cpp
                       src/commands/GSSAcquireCred.cpp 
                       src/commands/GSSDisplayName.cpp
                       src/commands/GSSPseudoRandom.cpp
                       src/commands/GSSWrap.cpp  
                       src/commands/GSSUnwrap.cpp
                       src/utils/util_base64.cpp
                       src/utils/util_json.cpp
                       src/utils/util_random.cpp
                       src/cache/GSSContextCache.cpp
                       src/cache/GSSCredentialCache.cpp
                       src/cache/GSSNameCache.cpp
                       src/datamodel/GSSContext.cpp
                      )
set_target_properties( jsongssapi
  	PROPERTIES SOVERSION 1)


GENERATE_EXPORT_HEADER(jsongssapi)

add_executable(json_gssapi src/main.cpp)

find_package (PkgConfig)
pkg_check_modules (GLIB2   glib-2.0)

find_package (Jansson)
if (JANSSON_FOUND)
else (JANSSON_FOUND)
  set(JANSSON_LIBRARIES jansson)
endif (JANSSON_FOUND)

# deal with gssapi / krb5
if (WIN32)
  if ("$ENV{TARGET_CPU}" STREQUAL "x86")
    set(KRB5_SDK $ENV{ProgramFiles}\\MIT\\Kerberos)
    set(GSSAPI_LIB ${KRB5_SDK}\\lib\\i386\\gssapi32.lib)
  elseif ("$ENV{TARGET_CPU}" STREQUAL "x64")
    set(KRB5_SDK $ENV{PROGRAMW6432}\\MIT\\Kerberos)
    set(GSSAPI_LIB ${KRB5_SDK}\\lib\\amd64\\gssapi64.lib)
  else ("$ENV{TARGET_CPU}" STREQUAL "x86")
    message(FATAL_ERROR "TARGET_CPU '$ENV{TARGET_CPU}' not supported. Did you run setenv.bat?")
  endif ("$ENV{TARGET_CPU}" STREQUAL "x86")
  set(KRB5_INCLUDE_DIRS ${KRB5_SDK}\\include;${KRB5_SDK}\\include\\gssapi)
  set(KRB5_LIBRARIES ${GSSAPI_LIB})
else (WIN32)
  set(KRB5_LIBRARIES gssapi_krb5)
endif (WIN32)

# openssl / crpyto for random
if (WIN32)
else (WIN32)
target_link_libraries(jsongssapi crypto)
target_link_libraries(json_gssapi crypto)
add_definitions(-DHAVE_OPENSSL)
endif (WIN32)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /WX")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
endif ()

include_directories(${KRB5_INCLUDE_DIRS} ${JANSSON_INCLUDE_DIRS})
target_link_libraries(jsongssapi ${KRB5_LIBRARIES} ${JANSSON_LIBRARIES})
target_link_libraries(json_gssapi jsongssapi ${KRB5_LIBRARIES} ${JANSSON_LIBRARIES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")

find_package (CPPUNIT)
install(TARGETS json_gssapi jsongssapi
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)

add_subdirectory(src)

add_subdirectory(test)
