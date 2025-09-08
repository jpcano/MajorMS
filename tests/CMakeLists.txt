set(PROJECT_TEST_NAME test_majorMS)

enable_testing()
add_executable(
  ${PROJECT_TEST_NAME}
  hello_test.cpp
  string_number.cpp
)
target_include_directories(${PROJECT_TEST_NAME} PRIVATE ../src)
target_link_libraries(${PROJECT_TEST_NAME} majorms_lib)
target_link_libraries(
  ${PROJECT_TEST_NAME}
  GTest::gtest_main
)
include(GoogleTest)
gtest_discover_tests(${PROJECT_TEST_NAME})

install(TARGETS ${PROJECT_TEST_NAME})
