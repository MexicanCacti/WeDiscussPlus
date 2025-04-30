file(GLOB SERVER_FILES *.cpp)

set(SERVER_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

message(STATUS "Building WeDiscussServer")

add_executable(WeDiscussServer ${SERVER_FILES} ${SERVER_INCLUDE_HEADERS} ${SHARED_INCLUDE_HEADERS}) 

target_link_libraries(WeDiscussServer PRIVATE core)

set_target_properties(WeDiscussServer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${SERVER_OUTPUT_DIR}
)