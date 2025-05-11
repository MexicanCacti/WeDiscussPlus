file(GLOB MESSAGE_TEST_FILES ${CMAKE_SOURCE_DIR}/tests/message_test/*.cpp)

set(MESSAGE_TEST_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

add_executable(message_test ${MESSAGE_TEST_FILES})

set_target_properties(message_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${MESSAGE_TEST_OUTPUT_DIR}
)

target_link_libraries(message_test PRIVATE core GTest::gtest_main)

target_include_directories(message_test PRIVATE ${SHARED_INCLUDE_HEADERS})

target_compile_definitions(message_test PRIVATE _DEBUG)

include(GoogleTest)
gtest_discover_tests(message_test)