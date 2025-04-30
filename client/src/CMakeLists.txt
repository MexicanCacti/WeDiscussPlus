file(GLOB CLIENT_FILES *.cpp)

set(CLIENT_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

message(STATUS "Building WeDiscussClient")

add_executable(WeDiscussClient ${CLIENT_FILES})

target_link_libraries(WeDiscussClient PRIVATE core)

target_include_directories(WeDiscussClient PRIVATE ${SHARED_INCLUDE_HEADERS} ${CLIENT_INCLUDE_HEADERS})

set_target_properties(WeDiscussClient PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CLIENT_OUTPUT_DIR}
)