cmake_minimum_required(VERSION 2.8)

project(json_gssapi)

add_executable(json_gssapi src/GSSCreateSecContextCommand.cpp src/GSSCommand.cpp src/util_json.cpp main.cpp)
target_link_libraries(json_gssapi gssapi_krb5 jansson)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")
FIND_PACKAGE(CPPUNIT REQUIRED)

install(TARGETS json_gssapi RUNTIME DESTINATION bin)

add_subdirectory(src)

add_subdirectory(test)