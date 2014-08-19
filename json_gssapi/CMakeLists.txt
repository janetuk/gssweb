cmake_minimum_required(VERSION 2.8)

project(json_gssapi)

include_directories(src/)

add_executable(json_gssapi src/datamodel/GSSName.cpp
                           src/datamodel/GSSOID.cpp
                           src/datamodel/GSSOIDSet.cpp
                           src/datamodel/GSSBuffer.cpp
                           src/datamodel/GSSCredential.cpp
                           src/GSSException.cpp
                           src/commands/GSSGetMic.cpp
                           src/commands/GSSCreateSecContextCommand.cpp
                           src/commands/GSSCommand.cpp 
                           src/commands/GSSImportName.cpp
                           src/commands/GSSAcquireCred.cpp 
                           src/commands/GSSPseudoRandom.cpp
                           src/commands/GSSWrap.cpp  
                           src/commands/GSSUnwrap.cpp
                           src/util_json.cpp 
                           main.cpp)
target_link_libraries(json_gssapi gssapi_krb5 jansson)


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules/")
FIND_PACKAGE(CPPUNIT REQUIRED)

install(TARGETS json_gssapi RUNTIME DESTINATION bin)

add_subdirectory(src)

add_subdirectory(test)