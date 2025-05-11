include(FetchContent)
FetchContent_Declare(
  google_test
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        main
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(google_test)


include_directories(${CMAKE_SOURCE_DIR}/tests)

add_subdirectory(mock_objects)
add_subdirectory(message_test)
add_subdirectory(routing_test)