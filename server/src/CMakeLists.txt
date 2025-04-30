file(GLOB SERVER_FILES *.cpp)

set(SERVER_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

message(STATUS "Building WeDiscussServer")

add_executable(WeDiscussServer ${SERVER_FILES}) 

target_link_libraries(WeDiscussServer PRIVATE core)

target_include_directories(WeDiscussServer PRIVATE ${SHARED_INCLUDE_HEADERS} ${SERVER_INCLUDE_HEADERS})

set_target_properties(WeDiscussServer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${SERVER_OUTPUT_DIR}
)