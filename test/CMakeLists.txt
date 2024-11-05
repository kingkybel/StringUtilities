add_executable(run_tests
        run_tests.cc
        stringutil_tests.cc
        brackets_tests.cc
        decorator_tests.cc
)

target_link_libraries(run_tests
        gtest
        gtest_main
)
add_test(NAME run_tests COMMAND run_tests)

