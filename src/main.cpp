#include <utf8.h>
#include <utf8/checked.h>

#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "major.h"
#include "string_number.h"

int main(int argc, char** argv) {
  cxxopts::Options options("majorMS", "Major Mnemonic System");

  options.add_options()("v,version", "Show version")("h,help", "Print usage")(
      "numbers", "The number", cxxopts::value<std::vector<std::string>>())(
      "csv", "Save to CSV file", cxxopts::value<std::string>())(
      "merged", "Used merged search strategy", cxxopts::value<bool>());

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

  Major major({
      {"en", "../data/config_en_UK.json", "../data/en_UK.txt"},
      {"es", "../data/config_es_ES.json", "../data/es_ES.txt"},
  });

  SearchType st =
      result.count("merged") ? SearchType::Merged : SearchType::Separated;

  if (result.count("csv")) {
    major.saveWords(result["csv"].as<std::string>(), numbers[0], numbers[1],
                    st);
    exit(0);
  }

  for (StringNumber n(numbers[0]); n <= StringNumber(numbers[1]); n++) {
    std::cout << n.get() << " ===============================" << std::endl
              << std::endl;
    auto results = major.findWords(n.get(), st);
    if (results.size() > 0) {
      int i, j;
      j = 0;
      for (auto& result : results) {
        for (auto& words : result) {
          i = 0;
          for (auto& word : words.data) {
            std::cout << word.name << " (" << word.ipa << ")";
            if (i++ < words.data.size() - 1) std::cout << ", ";
          }
          std::cout << std::endl << std::endl;
        }
        if ((results.size() - 1) != j++)
          std::cout << "---" << std::endl << std::endl;
      }
    }
    if (n != StringNumber(numbers[1])) std::cout << std::endl;
  }

  return 0;
}
