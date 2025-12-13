cmake_minimum_required(VERSION 3.20)

set(T edu_vec)

project(${T})

set(CMAKE_C_STANDARD 11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -pedantic)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fsanitize=address,leak,undefined")

include_directories(${CMAKE_SOURCE_DIR}/include)

add_library(
        ${T} SHARED
        src/edu_vec.c
        src/edu_print.c
        src/edu_cmp.c
)

#enable_testing()

add_subdirectory(test)