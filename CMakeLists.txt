cmake_minimum_required(VERSION 3.14)
project(WeDiscuss++ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_compile_options("/FS")
# Define _WIN32_WINNT for Windows, 0x0601 = Windows 7, 0x0A00 = Windows 10
add_definitions(-D_WIN32_WINNT=0x0A00)

enable_testing()

#Put CMake Build Files in Temp Folder
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/temp)

#Set Binary & Library Output Folders
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/lib)

#Source Files
file(GLOB_RECURSE INCLUDE_SOURCES shared_files/*.cpp)
file(GLOB_RECURSE CLIENT_INCLUDE_HEADERS include/client_include/*.hpp)
file(GLOB_RECURSE SERVER_INCLUDE_HEADERS include/server_include/*.hpp)
file(GLOB_RECURSE SHARED_INCLUDE_HEADERS include/shared_include/*.hpp)

#Include Directories
include_directories(include libs)

#Add Libraries
add_library(core STATIC ${INCLUDE_SOURCES})

add_library(asio INTERFACE)

target_compile_definitions(asio INTERFACE ASIO_STANDALONE)
target_include_directories(asio INTERFACE ${CMAKE_SOURCE_DIR}/libs/asio-1.30.2/include)

target_include_directories(core PUBLIC 
    ${CMAKE_SOURCE_DIR}/include/client_include
    ${CMAKE_SOURCE_DIR}/include/server_include
    ${CMAKE_SOURCE_DIR}/include/shared_include
)

target_link_libraries(core PUBLIC asio)

if(UNIX)
    find_package(Threads REQUIRED)
    target_link_libraries(asio INTERFACE Threads::Threads)
endif()

#Recursively build other projects
add_subdirectory(demo_client)
add_subdirectory(demo_server)
add_subdirectory(tests)

#Private: Only files in project need the library/include
#Public:  (Libary/Include) need as dependency AND files call functions from it
#Interface: Libary/include is only dependency of another library/include