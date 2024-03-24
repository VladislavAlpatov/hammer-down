cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_SHARED_MODULE_PREFIX "")

if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug")
else ()
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
endif ()


if(WIN32)
    set(CMAKE_CXX_COMPILER cl.exe)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out")
elseif(UNIX)
    set(CMAKE_CXX_COMPILER g++)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out/${CMAKE_BUILD_TYPE}")
endif()

# list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

project(hammer-down)
add_library(hammer-down STATIC source/HammerDown.cpp)

target_include_directories(hammer-down PUBLIC include)
target_include_directories(hammer-down PRIVATE source)

target_compile_definitions(hammer-down PRIVATE WIN32_LEAN_AND_MEAN)

target_link_libraries(hammer-down PRIVATE asio)

add_subdirectory(source)
add_subdirectory(extlibs)
add_subdirectory(tests)
