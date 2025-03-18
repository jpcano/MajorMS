#include "phonemes.h"

#include <utf8/checked.h>
#include <utf8/cpp11.h>

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

Phonemes::Phonemes(std::string config_path) : _config_path(config_path) {
  std::ifstream fi(_config_path);
  _config = json::parse(fi);

  std::string _config_str = _config.dump(2);
  _config_hash = XXH3_128bits(_config_str.c_str(), strlen(_config_str.c_str()));

  std::ofstream fo(_config_path, std::ofstream::out);
  fo << _config_str;

  for (auto& i : _config["ignore"]) {
    std::string i_str(i);
    std::vector<utf8::utfchar32_t> key;
    std::string::iterator it = i_str.begin();

    for (int i = 0; i < utf8::distance(i_str.begin(), i_str.end()); i++) {
      key.push_back(utf8::next(it, i_str.end()));
    }
    _ignore.insert(key);
  }

  for (auto& i : _config["table"].items())
    for (auto& j : i.value()) {
      std::string j_str(j);
      std::vector<utf8::utfchar32_t> key;
      std::string::iterator it = j_str.begin();
      for (int i = 0; i < utf8::distance(j_str.begin(), j_str.end()); i++) {
        key.push_back(utf8::next(it, j_str.end()));
      }
      _table.insert({key, i.key()});
    }
}

std::string Phonemes::getNumber(std::vector<utf8::utfchar32_t> ch,
                                std::string word, std::string ipa) {
  if (isIgnore(ch) || !_table.count(ch)) {
    std::string target = "[ ";
    for (auto& c : ch) {
      target += std::to_string(c) + " ";
    }
    target += "]";
    throw std::out_of_range(
        "Character " + target + " (UTF32-decimal) in the word " + word + " (" +
        ipa + ") " + "does not exist in the table, consider adding it.");
  } else {
    return _table.at(ch);
  }
}

bool Phonemes::isIgnore(std::vector<utf8::utfchar32_t> ch) {
  return bool(_ignore.count(ch));
}

void Phonemes::printConfig() { std::cout << _config.dump(4) << std::endl; }

int Phonemes::nextPhoneme(std::string::iterator& it, std::string::iterator end,
                          std::vector<utf8::utfchar32_t>& ch) {
  int i = 1;
  utf8::utfchar32_t c = utf8::next(it, end);
  ch.push_back(c);

  // Detect and extract affricates
  // https://en.wikipedia.org/wiki/International_Phonetic_Alphabet#Affricates

  // tʃ
  if (c == 116 && utf8::distance(it, end) >= 1) {
    if (utf8::peek_next(it, end) == 643) {
      ch.push_back(utf8::next(it, end));
      i++;
    }
  }
  // dʒ
  if (c == 100 && utf8::distance(it, end) >= 1) {
    if (utf8::peek_next(it, end) == 658) {
      ch.push_back(utf8::next(it, end));
      i++;
    }
  }
  return i;
}
