file(GLOB CLIENT_FILES *.cpp)

set(CLIENT_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

message(STATUS "Building [DEMO] WeDiscussClient")

add_executable(WeDiscussClientDemo ${CLIENT_FILES})

target_link_libraries(WeDiscussClientDemo PRIVATE core)

target_include_directories(WeDiscussClientDemo PRIVATE ${SHARED_INCLUDE_HEADERS})

set_target_properties(WeDiscussClientDemo PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CLIENT_OUTPUT_DIR}
)