# Create a library for mock objects
add_library(mock_objects INTERFACE)

# Add include directories
target_include_directories(mock_objects INTERFACE
    ${CMAKE_SOURCE_DIR}/tests/mock_objects
    ${SHARED_INCLUDE_HEADERS}
    ${SERVER_INCLUDE_HEADERS}
    ${CMAKE_SOURCE_DIR}/libs/SQLiteCpp/include
) 