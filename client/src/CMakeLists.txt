file(GLOB CLIENT_FILES *.cpp)

set(CLIENT_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

message(STATUS "Building WeDiscussClient")

add_executable(WeDiscussClient ${CLIENT_FILES} ${CLIENT_INCLUDE_HEADERS} ${SHARED_INCLUDE_HEADERS})

target_link_libraries(WeDiscussClient PRIVATE core)

set_target_properties(WeDiscussClient PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CLIENT_OUTPUT_DIR}
)