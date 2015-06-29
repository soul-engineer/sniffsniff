#------------------------------------------------------------------------------#
#                            General section                                   #
#------------------------------------------------------------------------------#
# Sources
set (HEADERS
    ${HEADERS}
)
set (SOURCES
    ${SOURCES}
)

# Build a binary
add_executable (
        ${PROJECT}
        ${HEADERS}
        ${SOURCES} ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)
target_link_libraries (${PROJECT}
        ${Boost_LIBRARIES}
        ${GLOG_LIBRARY}
        ${CMAKE_THREAD_LIBS_INIT}
)

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
#                           Unit tests                                         #
#------------------------------------------------------------------------------#
set (TEST_NAME ${PROJECT}_test)

# Test sources
set (TEST_HEADERS
    ${CATCH_HEADER}
)
set (TEST_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/main_test.cpp
)

# Build a test binary
add_executable (
        ${TEST_NAME}
        ${HEADERS} ${SOURCES}
        ${TEST_HEADERS} ${TEST_SOURCES}
)

# Add a binary to the test system
add_test (
        NAME ${TEST_NAME}
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TEST_NAME}
)