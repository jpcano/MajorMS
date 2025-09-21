cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_PREFIX_PATH "/home/suso/Qt/6.7.3/gcc_64")

project(majorMS VERSION 1.0.0 LANGUAGES CXX)
set(PROJECT_UI_NAME majorMS_UI)

find_package(cxxopts REQUIRED)
find_package(utf8cpp REQUIRED)
find_package(vincentlaucsb-csv-parser REQUIRED)
find_package(GTest REQUIRED)
find_package(Qt6
  REQUIRED COMPONENTS Core Gui Widgets
  OPTIONAL_COMPONENTS PrintSupport
)
find_package(cereal REQUIRED)

add_subdirectory(src)
add_subdirectory(tests)

install(FILES
  data/en_UK.txt
  data/es_ES.txt
  DESTINATION data
)

SET(CPACK_OUTPUT_FILE_PREFIX packages)
include(CPack)

include(CTest)
