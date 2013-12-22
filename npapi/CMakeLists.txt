cmake_minimum_required (VERSION 2.8)

project (GSSWeb)

#include_directories ("${PROJECT_SOURCE_DIR}/Win")
#add_subdirectory (Win) 

# add the library
add_library (GSSWeb SHARED npapiplugin.cpp js_gss_api.c npapi.c)
#target_link_libraries (GSSWeb Win)
