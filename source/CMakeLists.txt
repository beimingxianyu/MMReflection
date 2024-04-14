include(CheckCXXCompilerFlag)

file(GLOB_RECURSE all_source_files "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
add_library(mm_reflection SHARED ${all_source_files})
target_include_directories(mm_reflection PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
#check_cxx_compiler_flag(-fPIC COMPILER_SUPPORTS_PIC)
#if (COMPILER_SUPPORTS_PIC)
#    target_compile_options(mm_reflection PRIVATE -fPIC)
#endif ()
