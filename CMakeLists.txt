cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(majorMS)
set(PROJECT_TEST_NAME test_majorMS)

find_package(cxxopts REQUIRED)
find_package(utf8cpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(vincentlaucsb-csv-parser REQUIRED)
find_package(xxHash REQUIRED)
find_package(GTest REQUIRED)

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/phonemes.cpp
  src/dictionary.cpp
  src/major.cpp
  src/string_number.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} cxxopts::cxxopts)
target_link_libraries(${PROJECT_NAME} utf8cpp)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)
target_link_libraries(${PROJECT_NAME} vincentlaucsb-csv-parser::vincentlaucsb-csv-parser)
target_link_libraries(${PROJECT_NAME} xxHash::xxhash)

enable_testing()

add_executable(
  ${PROJECT_TEST_NAME}
  tests/hello_test.cpp
  tests/string_number.cpp
  src/string_number.cpp
)
target_include_directories(${PROJECT_TEST_NAME} PRIVATE src)
target_link_libraries(
  ${PROJECT_TEST_NAME}
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(${PROJECT_TEST_NAME})
