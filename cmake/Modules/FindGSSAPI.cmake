#.rst:
# FindGSSAPI
# -------
# Finds the gssapi library
#
# This will will define the following variables::
#
# GSSAPI_FOUND - system has gssapi
# GSSAPI_INCLUDE_DIRS - the gssapi include directory
# GSSAPI_LIBRARIES - the gssapi libraries

find_library(GSSAPI_LIBRARY NAMES gssapi_krb5 gssapi
                            HINTS "${CMAKE_PREFIX_PATH}/lib")

find_path(GSSAPI_INCLUDE_DIR NAMES gssapi.h
                                   gssapi/gssapi.h
                             HINTS "${CMAKE_PREFIX_PATH}/include")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GSSAPI
                                  REQUIRED_VARS GSSAPI_LIBRARY GSSAPI_INCLUDE_DIR)

if (GSSAPI_LIBRARY AND GSSAPI_INCLUDE_DIRS)
  set(GSSAPI_FOUND TRUE)
endif ()

if(GSSAPI_FOUND)
  set(GSSAPI_LIBRARIES ${GSSAPI_LIBRARY})
  set(GSSAPI_INCLUDE_DIRS ${GSSAPI_INCLUDE_DIR})
  set(CMAKE_REQUIRED_INCLUDES ${GSSAPI_INCLUDE_DIRS})
endif()

mark_as_advanced(GSSAPI_LIBRARIES GSSAPI_INCLUDE_DIRS)
