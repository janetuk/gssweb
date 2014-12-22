cmake_minimum_required(VERSION 2.8)
include(GenerateExportHeader)
project(json_gssapi)

include_directories(src/ ${CMAKE_CURRENT_BINARY_DIR})

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
                       src/util_base64.cpp
                       src/util_json.cpp
                       src/util_random.cpp
                       src/cache/GSSContextCache.cpp
                       src/cache/GSSNameCache.cpp
                       src/datamodel/GSSContext.cpp
                      )

GENERATE_EXPORT_HEADER(jsongssapi)

add_executable(json_gssapi main.cpp)

find_package (PkgConfig)
pkg_check_modules (GLIB2   glib-2.0)

find_package (Jansson)
if (JANSSON_FOUND)
else (JANSSON_FOUND)
  set(JANSSON_LIBRARIES jansson)
endif (JANSSON_FOUND)

# deal with gssapi / krb5
if (WIN32)
set(KRB5_SDK $ENV{PROGRAMW6432}\\MIT\\Kerberos)
message("KRB5_SDK: ${KRB5_SDK}" )
set(KRB5_INCLUDE_DIRS ${KRB5_SDK}\\include;${KRB5_SDK}\\include\\gssapi)
set(KRB5_LIBRARIES ${KRB5_SDK}\\lib\\amd64\\gssapi64.lib)
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
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib)

add_subdirectory(src)

add_subdirectory(test)
