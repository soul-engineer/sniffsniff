#------------------------------------------------------------------------------#
#                            Project settings                                  #
#------------------------------------------------------------------------------#
cmake_minimum_required (VERSION 2.8.3)
set (PROJECT sniffsniff)

project (${PROJECT})

# External sources directory
set (EXTERNAL_SOURCES_DIR
        ${CMAKE_CURRENT_LIST_DIR}/external)

#------------------------------------------------------------------------------#
#                           Compiler settings                                  #
#------------------------------------------------------------------------------#
set (CMAKE_CXX_FLAGS
        -std=c++11)

#------------------------------------------------------------------------------#
#                        Configuration summary                                 #
#------------------------------------------------------------------------------#
# The whole idea of CMake configuration summary is taken from the PCRE project
message (STATUS "")
message (STATUS "${PROJECT} configuration summary:")
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
include_directories (${CATCH_INCLUDE_DIR})
set (HEADER_FILES
        ${CATCH_INCLUDE_DIR}/catch.hpp
)
enable_testing()

#------------------------------------------------------------------------------#
#                             Source files                                     #
#------------------------------------------------------------------------------#
add_subdirectory (${CMAKE_CURRENT_LIST_DIR}/src)