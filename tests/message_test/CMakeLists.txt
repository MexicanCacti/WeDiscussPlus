file(GLOB MESSAGE_TEST_FILES *.cpp)

set(MESSAGE_TEST_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

enable_testing()

message(STATUS "Building message_test")

add_executable(message_test ${MESSAGE_TEST_FILES} ${SHARED_INCLUDE_HEADERS})

# Needed b/c GoogleTest's CMakeFiles outputs lib files to same directory as temporary CMake files
link_directories(${CMAKE_BINARY_DIR}/lib)

target_link_libraries(message_test PRIVATE core GTest::gtest_main)

target_include_directories(message_test PRIVATE ${CMAKE_SOURCE_DIR}/include)

set_target_properties(message_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${MESSAGE_TEST_OUTPUT_DIR}
)

include(GoogleTest)

gtest_discover_tests(message_test)