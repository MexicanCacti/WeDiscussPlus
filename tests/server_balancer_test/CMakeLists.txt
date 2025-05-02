file(GLOB MESSAGE_TEST_FILES ${CMAKE_SOURCE_DIR}/tests/server_balancer_test/*.cpp)
file(GLOB MOCK_TEST_FILES ${CMAKE_SOURCE_DIR}/tests/mock_objects/*.cpp)
file(GLOB MOCK_TEST_HEADER_FILES ${CMAKE_SOURCE_DIR}/tests/mock_objects/*.hpp)

set(MESSAGE_TEST_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

add_executable(server_balancer_test ${MESSAGE_TEST_FILES} ${MOCK_TEST_HEADER_FILES} ${MOCK_TEST_FILES})

set_target_properties(server_balancer_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${MESSAGE_TEST_OUTPUT_DIR}
)

target_link_libraries(server_balancer_test PRIVATE core GTest::gtest_main)

target_include_directories(server_balancer_test PRIVATE ${SHARED_INCLUDE_HEADERS} ${SERVER_INCLUDE_HEADERS})

include(GoogleTest)
gtest_discover_tests(server_balancer_test)