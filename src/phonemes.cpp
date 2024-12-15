#include "phonemes.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

Phonemes::Phonemes(std::string config_path) : _config_path(config_path) {
  std::ifstream f(_config_path);
  _config = json::parse(f);

  for (auto& i : _config["ignore"]) {
    std::string i_str(i);
    std::string::iterator it = i_str.begin();
    if (utf8::distance(i_str.begin(), i_str.end()) == 1) {
      _ignore.insert(utf8::next(it, i_str.end()));
    }
  }

  for (auto& i : _config["table"].items())
    for (auto& j : i.value()) {
      std::string j_str(j);
      std::string::iterator it = j_str.begin();
      if (utf8::distance(j_str.begin(), j_str.end()) == 1) {
        _table.insert({utf8::next(it, j_str.end()), i.key()});
      }
    }
}

std::string Phonemes::getNumber(utf8::utfchar32_t ch) {
  if (isIgnore(ch) || !_table.count(ch)) {
    throw std::out_of_range(
        "Character `" + std::to_string(ch) +
        "` (UTF32-decimal) does not exist in the table, consider adding it.");
  } else {
    return _table.at(ch);
  }
}

bool Phonemes::isIgnore(utf8::utfchar32_t ch) {
  return bool(_ignore.count(ch));
}

void Phonemes::printConfig() { std::cout << _config.dump(4) << std::endl; }
