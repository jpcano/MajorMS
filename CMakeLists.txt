cmake_minimum_required(VERSION 3.23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(majorMS VERSION 1.0.0 LANGUAGES CXX)

set(PROJECT_UI_NAME majorMS_UI)

find_package(cxxopts REQUIRED)
find_package(utf8cpp REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(vincentlaucsb-csv-parser REQUIRED)
find_package(xxHash REQUIRED)
find_package(GTest REQUIRED)
find_package(Qt6
  REQUIRED COMPONENTS Core Gui Widgets
  OPTIONAL_COMPONENTS PrintSupport
)

add_subdirectory(src)
add_subdirectory(tests)

install(FILES
  data/en_UK.txt
  data/config_en_UK.json
  data/es_ES.txt
  data/config_es_ES.json
  DESTINATION data
)
include(CPack)
