#include <utf8.h>

#include <cxxopts.hpp>
#include <fstream>
#include <iostream>
#include <string>

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

  std::ifstream file("../data/es_ES.txt");
  std::string str;
  while (std::getline(file, str)) {
    std::cout << str << std::endl;

    std::cout << "Looping the std::string" << std::endl;
    for (char& ch : str) {
      std::cout << ch << std::endl;
    }

    std::cout << "Looping the uf8" << std::endl;
    std::string::iterator it = str.begin();
    for (int i = 0; i < utf8::distance(str.begin(), str.end()); i++) {
      utf8::utfchar32_t cp = utf8::next(it, str.end());
      std::cout << cp << std::endl;
    }
  }
  return 0;
}
