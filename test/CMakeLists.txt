project(MMReflectionTest)

enable_testing()

file(GLOB_RECURSE all_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(mm_reflection_test ${all_source_files})
target_link_libraries(mm_reflection_test PRIVATE gtest_main mm_reflection)
if (WIN32)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        set(LIB_PATH ${CMAKE_CURRENT_BINARY_DIR}/../source/mm_reflection.dll)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        set(LIB_PATH ${CMAKE_CURRENT_BINARY_DIR}/../source/libmm_reflection.dll)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(LIB_PATH ${CMAKE_CURRENT_BINARY_DIR}/../source/mm_reflection.dll)
    endif()
    set(TARGET_COPY_DIR ${CMAKE_CURRENT_BINARY_DIR})

    add_custom_command(
            TARGET mm_reflection_test POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${LIB_PATH}
            ${TARGET_COPY_DIR}
            COMMENT "Copy the DLL to ${TARGET_COPY_DIR} to test program can run."
    )
endif ()

include(GoogleTest)
gtest_add_tests(TARGET mm_reflection_test)
