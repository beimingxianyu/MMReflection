cmake_minimum_required(VERSION 3.8)

project(MMReflection)

option(MM_REFLECTION_ENABLE_TEST "Whether to open unit test." ON)

set(CMAKE_CXX_STANDARD 17)
if (WIN32)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS on)
endif ()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/3rd_party")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/source")

if (MM_REFLECTION_ENABLE_TEST)
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/test")
endif()
