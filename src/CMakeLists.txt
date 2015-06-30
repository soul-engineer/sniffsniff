#------------------------------------------------------------------------------#
#                                 Boost                                        #
#------------------------------------------------------------------------------#
find_package (
        Boost
        REQUIRED
        COMPONENTS system program_options
)
include_directories (SYSTEM ${Boost_INCLUDE_DIRS})

#------------------------------------------------------------------------------#
#                                 Threads                                      #
#------------------------------------------------------------------------------#
find_package (
        Threads
        REQUIRED
)

#------------------------------------------------------------------------------#
#                                  libpcap                                     #
#------------------------------------------------------------------------------#
# libpcap library
find_library (PCAP_LIBRARY pcap)
if (NOT PCAP_LIBRARY)
        message (FATAL_ERROR "libpcap library is not found")
endif ()

# libpcap includes
find_path (PCAP_INCLUDE_DIR pcap/pcap.h)
if (NOT PCAP_INCLUDE_DIR)
        message (FATAL_ERROR "libpcap includes are not found")
endif ()

message (STATUS "libpcap network traffic capture library:")
message (STATUS "  library is found at ${PCAP_LIBRARY}")
message (STATUS "  includes are found at ${PCAP_INCLUDE_DIR}")

include_directories (SYSTEM ${PCAP_INCLUDE_DIR})

#------------------------------------------------------------------------------#
#                            General section                                   #
#------------------------------------------------------------------------------#
# Sources
set (HEADERS
        ${HEADERS}
        config.h
)
set (SOURCES
        ${SOURCES}
        config.cpp
)

# Build a binary
add_executable (
        ${PROJECT}
        ${HEADERS}
        ${SOURCES}
        main.cpp
)
target_link_libraries (${PROJECT}
        ${PCAP_LIBRARY}
        ${Boost_LIBRARIES}
        ${GLOG_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
)

#------------------------------------------------------------------------------#
#                           Unit tests                                         #
#------------------------------------------------------------------------------#
set (TEST_NAME ${PROJECT}_test)

# Test sources
set (TEST_HEADERS
    ${CATCH_HEADER}
)
set (TEST_SOURCES
        config_test.cpp
        main_test.cpp
)

# Build a test binary
add_executable (
        ${TEST_NAME}
        ${HEADERS} ${SOURCES}
        ${TEST_HEADERS} ${TEST_SOURCES}
)

target_link_libraries (${TEST_NAME}
        ${PCAP_LIBRARY}
        ${Boost_LIBRARIES}
        ${GLOG_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
)
set_target_properties(${TEST_NAME}
                      PROPERTIES COMPILE_DEFINITIONS _BUILT_WITH_CATCH 
)
# Add a binary to the test system
add_test (
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
)