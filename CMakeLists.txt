cmake_minimum_required(VERSION 3.14)
project(WeDiscuss++ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
include_directories(include)

#Add Libraries
add_library(core STATIC ${INCLUDE_SOURCES})

target_include_directories(core PUBLIC ${CMAKE_SOURCE_DIR}/include)

include(FetchContent)
FetchContent_Declare(
  google_test
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        main
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(google_test)

#Recursively build other projects
add_subdirectory(client)
add_subdirectory(server)
add_subdirectory(tests)


#Private: Only files in project need the library/include
#Public:  (Libary/Include) need as dependency AND files call functions from it
#Interface: Libary/include is only dependency of another library/include