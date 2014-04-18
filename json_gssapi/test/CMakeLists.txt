include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(test InitSecContextMock.cpp GSSCreateSecContextTest.cpp test_run.cpp JSONParseTest.cpp ../src/GSSCreateSecContextCommand.cpp ../src/util_json.cpp)

target_link_libraries(test cppunit gssapi_krb5 jansson)

# install(TARGETS test RUNTIME DESTINATION bin)
