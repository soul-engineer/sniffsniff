#------------------------------------------------------------------------------#
#                            Project settings                                  #
#------------------------------------------------------------------------------#
cmake_minimum_required (VERSION 2.8.3)
set (PROJECT sniffsniff)

project (${PROJECT})

# External sources directory
set (EXTERNAL_SOURCES_DIR
        ${CMAKE_CURRENT_LIST_DIR}/external)
# Binary output directory
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

#------------------------------------------------------------------------------#
#                           Compiler settings                                  #
#------------------------------------------------------------------------------#
set (CMAKE_CXX_FLAGS
        "-std=c++11 -Wall -Wextra")

#------------------------------------------------------------------------------#
#                        Configuration summary                                 #
#------------------------------------------------------------------------------#
# CMake configuration summary idea is taken from the PCRE project
message (STATUS "")
message (STATUS "${PROJECT} project configuration summary:")
message (STATUS "")
message (STATUS "  Build configuration ............. : ${CMAKE_BUILD_TYPE}")
message (STATUS "  Install prefix .................. : ${CMAKE_INSTALL_PREFIX}")
message (STATUS "  C compiler ...................... : ${CMAKE_C_COMPILER}")
message (STATUS "  C compiler flags ................ : ${CMAKE_C_FLAGS}${CMAKE_C_FLAGS_${buildtype}}")
message (STATUS "  C++ compiler .................... : ${CMAKE_CXX_COMPILER}")
message (STATUS "  C++ compiler flags .............. : ${CMAKE_CXX_FLAGS}${CMAKE_CXX_FLAGS_${buildtype}}")
message (STATUS "")

#------------------------------------------------------------------------------#
#                         Catch test framework                                 #
#------------------------------------------------------------------------------#
set (CATCH_INCLUDE_DIR
        ${EXTERNAL_SOURCES_DIR}/catch)
include_directories (SYSTEM ${CATCH_INCLUDE_DIR})
set (HEADER_FILES
        ${CATCH_INCLUDE_DIR}/catch.hpp
)
enable_testing()

#------------------------------------------------------------------------------#
#                           Google logging library                             #
#------------------------------------------------------------------------------#
# glog library
find_library (GLOG_LIBRARY glog)
if (NOT GLOG_LIBRARY)
        message (FATAL_ERROR "glog library is not found")
endif ()

# glog includes
find_path (GLOG_INCLUDE_DIR glog/logging.h)
if (NOT GLOG_INCLUDE_DIR)
        message (FATAL_ERROR "glog includes are not found")
endif ()

message (STATUS "Google logging library:")
message (STATUS "  glog library is found at ${GLOG_LIBRARY}")
message (STATUS "  glog includes are found at ${GLOG_INCLUDE_DIR}")

include_directories (SYSTEM ${GLOG_INCLUDE_DIR})

#------------------------------------------------------------------------------#
#                             Source files                                     #
#------------------------------------------------------------------------------#
add_subdirectory (${CMAKE_CURRENT_LIST_DIR}/src)