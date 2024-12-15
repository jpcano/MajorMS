#include <utf8.h>
#include <utf8/checked.h>

#include <cxxopts.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "dictionary.h"

int main(int argc, char** argv) {
  cxxopts::Options options("majorMS", "Major Mnemonic System");

  options.add_options()("v,version", "Show version")("h,help", "Print usage")(
      "number", "The number", cxxopts::value<std::string>());

  options.parse_positional({"number"});

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << std::endl;
    exit(0);
  }

  if (result.count("version")) {
    std::cout << "Development version" << std::endl;
    exit(0);
  }

  std::cout << "Hello World!" << std::endl;

  Dictionary dictionary("../data/config.json", "../data/es_ES.txt");

  for (auto& word : dictionary.getWords(result["number"].as<std::string>())) {
    std::cout << "Word: " << word.name << "    " << word.ipa << std::endl;
  }
  return 0;
}
