file(GLOB ROUTING_TEST_FILES ${CMAKE_SOURCE_DIR}/tests/routing_test/*.cpp)

set(ROUTING_TEST_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

add_executable(routing_test ${ROUTING_TEST_FILES})

set_target_properties(routing_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${ROUTING_TEST_OUTPUT_DIR}
)

target_link_libraries(routing_test PRIVATE 
    core 
    GTest::gtest_main
    mock_objects
    server
)

target_include_directories(routing_test PRIVATE 
    ${SHARED_INCLUDE_HEADERS} 
    ${SERVER_INCLUDE_HEADERS}
    ${CMAKE_SOURCE_DIR}/tests/mock_objects
    ${CMAKE_SOURCE_DIR}/tests
)

target_compile_definitions(routing_test PRIVATE _DEBUG _MOCK_TESTING)

include(GoogleTest)
gtest_discover_tests(routing_test)