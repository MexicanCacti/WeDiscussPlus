file(GLOB MESSAGE_TEST_FILES ${CMAKE_SOURCE_DIR}/tests/balancer_test/*.cpp)

set(MESSAGE_TEST_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

add_executable(balancer_test ${MESSAGE_TEST_FILES})

set_target_properties(balancer_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${MESSAGE_TEST_OUTPUT_DIR}
)

target_link_libraries(balancer_test PRIVATE core GTest::gtest_main)

target_include_directories(balancer_test PRIVATE ${SHARED_INCLUDE_HEADERS} ${SERVER_INCLUDE_HEADERS})

include(GoogleTest)
gtest_discover_tests(balancer_test)