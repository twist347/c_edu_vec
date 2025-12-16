cmake_minimum_required(VERSION 3.20)

project(edu_vec)

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CRITERION_DIR thirdparty/criterion-2.4.3)

add_library(
        edu_vec SHARED
        src/edu_vec.c
        src/edu_print.c
        src/edu_cmp.c
)

target_include_directories(edu_vec PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_compile_options(edu_vec PRIVATE -Wall -Wextra -pedantic)
target_link_libraries(edu_vec PRIVATE m)

option(EDU_VEC_BUILD_TESTS "Build tests" ON)
if (EDU_VEC_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif ()

option(EDU_VEC_SANITIZERS "Enable sanitizers for tests" ON)
