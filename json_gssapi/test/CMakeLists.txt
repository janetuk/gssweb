include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(test GSSExceptionTest.cpp 
                    GSSAcquireCredTest.cpp 
                    GSSGetMicTest.cpp
                    GSSPseudoRandomTest.cpp 
                    GSSWrapTest.cpp 
                    GSSUnwrapTest.cpp
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
                    ../src/GSSCreateSecContextCommand.cpp 
                    ../src/util_json.cpp 
                    ../src/GSSImportName.cpp
                    ../src/GSSException.cpp
                    ../src/GSSGetMic.cpp
                    ../src/GSSAcquireCred.cpp
                    ../src/GSSPseudoRandom.cpp
                    ../src/GSSUnwrap.cpp
                    ../src/GSSWrap.cpp
                    ../src/datamodel/GSSBuffer.cpp
                    ../src/datamodel/GSSCredential.cpp
                    ../src/datamodel/GSSName.cpp
                    ../src/datamodel/GSSOID.cpp
                    ../src/datamodel/GSSOIDSet.cpp
                    datamodel/GSSBufferTest.cpp
                    datamodel/GSSOIDSetTest.cpp
  )

target_link_libraries(test cppunit gssapi_krb5 jansson crypto)

# install(TARGETS test RUNTIME DESTINATION bin)

add_subdirectory(datamodel)
add_subdirectory(command_mocks)