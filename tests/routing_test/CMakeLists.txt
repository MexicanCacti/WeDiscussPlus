file(GLOB ROUTING_TEST_FILES ${CMAKE_SOURCE_DIR}/tests/routing_test/*.cpp)

set(ROUTING_TEST_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

# Create a separate server library target for testing
file(GLOB SERVER_TEST_SOURCES ${CMAKE_SOURCE_DIR}/server/src/*.cpp)
file(GLOB_RECURSE SHARED_INCLUDE_HEADERS include/shared_include/*.hpp)
file(GLOB_RECURSE MESSAGE_HEADERS include/shared_include/messages/*.hpp)

add_library(server_test STATIC ${SERVER_TEST_SOURCES} ${SHARED_INCLUDE_HEADERS} ${MESSAGE_HEADERS})
target_link_libraries(server_test PRIVATE core asio sqlite3)
target_include_directories(server_test PUBLIC 
    ${CMAKE_SOURCE_DIR}/include/server_include
    ${CMAKE_SOURCE_DIR}/include/shared_include
    ${CMAKE_SOURCE_DIR}/include/shared_include/messages
    ${CMAKE_SOURCE_DIR}/libs/asio-1.30.2/include
    ${CMAKE_SOURCE_DIR}/libs/sqlite3
)

# Define _DEBUG and _MOCK_TESTING for the test server library
target_compile_definitions(server_test PRIVATE _DEBUG _MOCK_TESTING)

add_executable(routing_test ${ROUTING_TEST_FILES})

set_target_properties(routing_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${ROUTING_TEST_OUTPUT_DIR}
)

# Define _MOCK_TESTING for the routing_test executable
target_compile_definitions(routing_test PRIVATE _MOCK_TESTING)

target_link_libraries(routing_test PRIVATE 
    core 
    GTest::gtest_main
    server_test
    sqlite3
)

target_include_directories(routing_test PRIVATE 
    ${SHARED_INCLUDE_HEADERS} 
    ${SERVER_INCLUDE_HEADERS}
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}/libs/sqlite3
)

include(GoogleTest)
gtest_discover_tests(routing_test)