add_library(majorms_lib STATIC
  dictionary.cpp
  major.cpp
  string_number.cpp
  conversion.cpp
)
target_link_libraries(majorms_lib utf8cpp)
target_link_libraries(majorms_lib vincentlaucsb-csv-parser::vincentlaucsb-csv-parser)
target_link_libraries(majorms_lib xxHash::xxhash)
target_link_libraries(majorms_lib cereal::cereal)


add_executable(${PROJECT_NAME}
  main.cpp
)
target_link_libraries(${PROJECT_NAME} cxxopts::cxxopts)
target_link_libraries(${PROJECT_NAME} majorms_lib)

install(TARGETS ${PROJECT_NAME})

add_subdirectory(ui)
