file(GLOB ROUTING_TEST_FILES ${CMAKE_SOURCE_DIR}/tests/routing_test/*.cpp)

set(ROUTING_TEST_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

add_executable(routing_test ${ROUTING_TEST_FILES})

set_target_properties(routing_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${ROUTING_TEST_OUTPUT_DIR}
)

# Define _MOCK_TESTING for both the test and the server library
add_definitions(-D_DEBUG)
target_compile_definitions(server PRIVATE _DEBUG)
add_definitions(-D_MOCK_TESTING)
target_compile_definitions(server PRIVATE _MOCK_TESTING)


target_link_libraries(routing_test PRIVATE 
    core 
    GTest::gtest_main
    mock_objects
    server
    sqlite3
)

target_include_directories(routing_test PRIVATE 
    ${SHARED_INCLUDE_HEADERS} 
    ${SERVER_INCLUDE_HEADERS}
    ${CMAKE_SOURCE_DIR}/tests/mock_objects
    ${CMAKE_SOURCE_DIR}/tests
    ${CMAKE_SOURCE_DIR}/libs/sqlite3
)

include(GoogleTest)
gtest_discover_tests(routing_test)