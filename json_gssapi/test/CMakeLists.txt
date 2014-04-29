include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(test GSSExceptionTest.cpp 
                    InitSecContextMock.cpp 
                    GSSCreateSecContextTest.cpp 
                    GSSImportNameTest.cpp
                    MockImportName.cpp
                    test_run.cpp 
                    ../src/GSSCreateSecContextCommand.cpp 
                    ../src/util_json.cpp 
                    ../src/GSSImportName.cpp
                    ../src/GSSException.cpp
                    ../src/datamodel/GSSBuffer.cpp
                    ../src/datamodel/GSSName.cpp
                    ../src/datamodel/GSSOID.cpp
                    datamodel/GSSBufferTest.cpp
  )

target_link_libraries(test cppunit gssapi_krb5 jansson)

# install(TARGETS test RUNTIME DESTINATION bin)

add_subdirectory(datamodel)