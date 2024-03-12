project(MMReflectionTest)

enable_testing()

file(GLOB_RECURSE all_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_executable(mm_reflection_test ${all_source_files})
target_link_libraries(mm_reflection_test PRIVATE gtest_main mm_reflection)

include(GoogleTest)
gtest_add_tests(TARGET mm_reflection_test)
