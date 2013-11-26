cmake_minimum_required (VERSION 2.8)

project (WebShot)

#include_directories ("${PROJECT_SOURCE_DIR}/Win")
#add_subdirectory (Win) 

# add the library
add_library (WebShot SHARED js_gss_api.c npapi.c)
#target_link_libraries (WebShot Win)
