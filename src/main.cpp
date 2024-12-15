#include <utf8.h>
#include <utf8/checked.h>

#include <cxxopts.hpp>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "phonemes.h"

struct Word {
  std::string name;
  std::string ipa;
};

int main(int argc, char** argv) {
  cxxopts::Options options("majorMS", "Major Mnemonic System");

  options.add_options()("v,version", "Show version")("h,help", "Print usage");

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

  std::map<int, std::vector<Word>> dictionary{};

  std::ifstream file("../data/es_ES.txt");
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    Word word;
    std::getline(iss, word.name, '\t');
    std::getline(iss, word.ipa, '\t');
    word.ipa.erase(0, 1);
    word.ipa.erase(word.ipa.size() - 1);
    int ipa_len = utf8::distance(word.ipa.begin(), word.ipa.end());
    if (!(dictionary.insert({ipa_len, {word}}).second)) {
      dictionary[ipa_len].push_back(word);
    }
  }

  for (auto& w : dictionary.at(8)) {
    std::cout << w.name << ": " << w.ipa << std::endl;
  };

  Phonemes p("../data/config.json");
  p.printConfig();
  try {
    std::string num;
    num = p.getNumber(107);  // k
    std::cout << "number: " << num << std::endl;
    num = p.getNumber(110);  // n
    std::cout << "number: " << num << std::endl;
    num = p.getNumber(111);  // o
    std::cout << "number: " << num << std::endl;
    num = p.getNumber(97);  // a
    std::cout << "number: " << num << std::endl;
    num = p.getNumber(122);  // z
    std::cout << "number: " << num << std::endl;
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}
