add_executable(test_edu_vec main.c)

target_include_directories(test_edu_vec PRIVATE ${CRITERION_DIR}/include)
target_link_directories(test_edu_vec PRIVATE ${CRITERION_DIR}/lib)
target_link_libraries(test_edu_vec PRIVATE edu_vec criterion)

add_library(edu_vec_san STATIC
        ${CMAKE_SOURCE_DIR}/src/edu_vec.c
        ${CMAKE_SOURCE_DIR}/src/edu_print.c
        ${CMAKE_SOURCE_DIR}/src/edu_cmp.c
)
target_include_directories(edu_vec_san PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(edu_vec_san PRIVATE m)

target_link_libraries(test_edu_vec PRIVATE edu_vec_san)

if (EDU_VEC_SANITIZERS)
    target_compile_options(test_edu_vec PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(test_edu_vec PRIVATE -fsanitize=address,undefined)

    target_compile_options(edu_vec_san PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
    target_link_options(edu_vec_san PRIVATE -fsanitize=address,undefined)
endif()

add_test(NAME test_edu_vec COMMAND test_edu_vec)
