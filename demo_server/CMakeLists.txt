file(GLOB SERVER_FILES *.cpp)

set(SERVER_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

message(STATUS "Building [DEMO] WeDiscussServer")

add_executable(WeDiscussServerDemo ${SERVER_FILES}) 

target_link_libraries(WeDiscussServerDemo PRIVATE core)

target_include_directories(WeDiscussServerDemo PRIVATE ${SHARED_INCLUDE_HEADERS})

set_target_properties(WeDiscussServerDemo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${SERVER_OUTPUT_DIR}
)