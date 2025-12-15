add_executable(test_edu_vec main.c)
target_include_directories(test_edu_vec PRIVATE ${CRITERION_DIR}/include)
target_link_directories(test_edu_vec PRIVATE ${CRITERION_DIR}/lib)
target_link_libraries(test_edu_vec PRIVATE edu_vec criterion)
