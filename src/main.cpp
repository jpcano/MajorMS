#include <utf8.h>
#include <utf8/checked.h>

#include <cereal/archives/json.hpp>
#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "dict_configs.h"
#include "dictionary.h"
#include "major.h"
#include "string_number.h"

int main(int argc, char** argv) {
  // clang-format off
  cxxopts::Options options(
    "majorMS",
    "Major Mnemonic System - Copyright (c) 2025 Jesus "
    "Cano <https://github.com/jpcano/MajorMS>"
  );

  options.add_options()
    ("numbers", "The number", cxxopts::value<std::vector<std::string>>())
    ("c,csv", "Save to CSV file", cxxopts::value<std::string>())
    ("d,dict", "A comma-sepparated list of dictionaries to use",
        cxxopts::value<std::vector<std::string>>()->default_value(get_dict_names_csv()))
    ("m,merged", "Used a merged search strategy", cxxopts::value<bool>())
    ("s,splits", "Start the search by spliting the number in `arg' parts",
        cxxopts::value<unsigned int>()->default_value("0"))
    ("t,tables", "Print conversion tables for each dictionary")
    ("v,version", "Show version")
    ("h,help", "Print usage");
  // clang-format on

  options.parse_positional({"numbers"});

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }

  if (result.count("version")) {
    std::cout << "Development version" << std::endl;
    exit(0);
  }

  if (result.count("tables")) {
    std::cout << get_conversion_tables() << std::endl;
    exit(0);
  }

  auto numbers = result["numbers"].as<std::vector<std::string>>();
  if (numbers.size() == 1) {
    try {
      StringNumber sn(numbers[0]);
      numbers.push_back(numbers[0]);
    } catch (...) {
      std::cout << "The argument provided is not a number" << std::endl;
      exit(0);
    }
  } else if (numbers.size() == 2) {
    try {
      StringNumber sn_start(numbers[0]);
      StringNumber sn_end(numbers[1]);

      if (sn_start > sn_end) {
        std::cout << "Start number should not be greater than the end number"
                  << std::endl;
        exit(0);
      }
    } catch (...) {
      std::cout << "The arguments provided are not numbers" << std::endl;
      exit(0);
    }
  } else {
    std::cout << "You should provide one or two numbers" << std::endl;
    exit(0);
  }

  if (result.count("splits")) {
    auto numbers = result["numbers"].as<std::vector<std::string>>();
    auto split = result["splits"].as<unsigned int>();
    if (split >= numbers[0].size()) {
      std::cout
          << "Cannot split the number in more parts than the number of digits"
          << std::endl;
      exit(0);
    }
  }

  std::vector<DictConfig> dicts;

  for (std::string dict : result["dict"].as<std::vector<std::string>>())
    dicts.push_back(dict_configs.configs.at(dict));

  std::ofstream oFile("dict_configs.json");
  cereal::JSONOutputArchive oArchive(oFile);
  oArchive(dict_configs);

  SearchType st =
      result.count("merged") ? SearchType::Merged : SearchType::Separated;
  auto splits = result["splits"].as<unsigned int>();

  Major major(dicts);

  if (result.count("csv")) {
    major.saveWords(result["csv"].as<std::string>(), numbers[0], numbers[1], st,
                    splits);
    exit(0);
  }

  for (StringNumber n(numbers[0]); n <= StringNumber(numbers[1]); n++) {
    std::cout << n.get() << " ===============================" << std::endl
              << std::endl;
    auto results = major.findWords(n.get(), st, splits);
    std::cout << Major::printResults(results);

    if (n != StringNumber(numbers[1])) std::cout << std::endl;
  }

  return 0;
}
