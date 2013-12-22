cmake_minimum_required (VERSION 2.8)

project (gssapi_utils)

# add the library
add_library (gssapi_utils SHARED gssapi_utils.c)
target_link_libraries (gssapi_utils gssapi_krb5)

