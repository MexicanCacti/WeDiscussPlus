file(GLOB SERVER_FILES *.cpp)

set(SERVER_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/build/bin)

message(STATUS "Building WeDiscussServer")

# Create server database directory in build for both Debug and Release
file(MAKE_DIRECTORY ${SERVER_OUTPUT_DIR}/Debug/server_database)
file(MAKE_DIRECTORY ${SERVER_OUTPUT_DIR}/Release/server_database)

# Copy schema file to both Debug and Release directories
file(COPY ${CMAKE_SOURCE_DIR}/include/server_database/schema.sql 
     DESTINATION ${SERVER_OUTPUT_DIR}/Debug/server_database)
file(COPY ${CMAKE_SOURCE_DIR}/include/server_database/schema.sql 
     DESTINATION ${SERVER_OUTPUT_DIR}/Release/server_database)

add_library(server STATIC ${SERVER_FILES})
target_link_libraries(server PRIVATE core asio sqlite3)
target_include_directories(server PUBLIC 
    ${CMAKE_SOURCE_DIR}/include/client_include
    ${CMAKE_SOURCE_DIR}/include/server_include
    ${CMAKE_SOURCE_DIR}/include/shared_include
    ${CMAKE_SOURCE_DIR}/include/shared_include/messages
    ${CMAKE_SOURCE_DIR}/libs/asio-1.30.2/include
    ${CMAKE_SOURCE_DIR}/libs/sqlite3
    ${CMAKE_SOURCE_DIR}/include/server_database
)

# Propagate _DEBUG flag if it's defined
if(DEFINED _DEBUG)
    target_compile_definitions(server PRIVATE _DEBUG)
endif()

add_executable(WeDiscussServer main.cpp)
target_link_libraries(WeDiscussServer PRIVATE server)

# Explicitly undefine _MOCK_TESTING for the main server executable
target_compile_definitions(WeDiscussServer PRIVATE -U_MOCK_TESTING)

target_include_directories(WeDiscussServer PRIVATE ${SHARED_INCLUDE_HEADERS} ${SERVER_INCLUDE_HEADERS})

set_target_properties(WeDiscussServer PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${SERVER_OUTPUT_DIR}
)

install(TARGETS WeDiscussServer
    RUNTIME DESTINATION bin
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/build/server_database/
    DESTINATION server_database
    FILES_MATCHING PATTERN "*.sql"
)

set(CPACK_PACKAGE_NAME "WeDiscussServer")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "WeDiscuss++ Server")
set(CPACK_GENERATOR "ZIP")
set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY FALSE)

include(CPack)