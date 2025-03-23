add_library(majorms_lib STATIC
  phonemes.cpp
  dictionary.cpp
  major.cpp
  string_number.cpp
)
target_link_libraries(majorms_lib utf8cpp)
target_link_libraries(majorms_lib nlohmann_json::nlohmann_json)
target_link_libraries(majorms_lib vincentlaucsb-csv-parser::vincentlaucsb-csv-parser)
target_link_libraries(majorms_lib xxHash::xxhash)

add_executable(${PROJECT_NAME}
  main.cpp
)
target_link_libraries(${PROJECT_NAME} cxxopts::cxxopts)
target_link_libraries(${PROJECT_NAME} majorms_lib)

