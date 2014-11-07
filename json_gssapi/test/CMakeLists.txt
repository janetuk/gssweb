include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/commands)

find_package (PkgConfig)
pkg_check_modules (GLIB2   glib-2.0)
if (GLIB2_FOUND)
  include_directories(${GLIB2_INCLUDE_DIRS})
endif(GLIB2_FOUND)
  

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
                    ../src/util_base64.cpp
                    ../src/util_json.cpp 
                    ../src/util_random.cpp
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

target_link_libraries(test cppunit gssapi_krb5 jansson crypto ${GLIB2_LIBRARIES})

# install(TARGETS test RUNTIME DESTINATION bin)

add_subdirectory(datamodel)
add_subdirectory(command_mocks)
