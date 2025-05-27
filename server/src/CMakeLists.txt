file(GLOB SERVER_FILES *.cpp)

set(SERVER_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

message(STATUS "Building WeDiscussServer")

add_library(server STATIC ${SERVER_FILES})
target_link_libraries(server PRIVATE core asio sqlite3)
target_include_directories(server PUBLIC 
    ${CMAKE_SOURCE_DIR}/include/client_include
    ${CMAKE_SOURCE_DIR}/include/server_include
    ${CMAKE_SOURCE_DIR}/include/shared_include
    ${CMAKE_SOURCE_DIR}/include/shared_include/messages
    ${CMAKE_SOURCE_DIR}/libs/asio-1.30.2/include
    ${CMAKE_SOURCE_DIR}/libs/sqlite3
)

# Propagate _DEBUG flag if it's defined
if(DEFINED _DEBUG)
    target_compile_definitions(server PRIVATE _DEBUG)
endif()

# Propagate _MOCK_TESTING flag if it's defined
if(DEFINED _MOCK_TESTING)
    target_compile_definitions(server PRIVATE _MOCK_TESTING)
endif()

add_executable(WeDiscussServer main.cpp)
target_link_libraries(WeDiscussServer PRIVATE server)

target_include_directories(WeDiscussServer PRIVATE ${SHARED_INCLUDE_HEADERS} ${SERVER_INCLUDE_HEADERS})

set_target_properties(WeDiscussServer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${SERVER_OUTPUT_DIR}
)