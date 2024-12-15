cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(majorMS)

find_package(cxxopts REQUIRED)
find_package(utf8cpp REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} cxxopts::cxxopts)
target_link_libraries(${PROJECT_NAME} utf8cpp)

