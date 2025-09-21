#include "dictionary.h"

#include <utf8.h>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

Dictionary::Dictionary(DictConfig config) {
  Conversion conv(config.table);
  std::ifstream file(config.dictionary_path);
  std::string line;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    Word word;

    word.lang = config.name;

    std::getline(iss, word.name, '\t');
    std::getline(iss, word.ipa, '\t');

    // Erase the '/' characters that surround the IPA pronunciation
    word.ipa.erase(0, 1);
    word.ipa.erase(word.ipa.size() - 1);

    std::string number = conv.phonetic_to_number(word.ipa);

    _longest = number.size() > _longest ? number.size() : _longest;

    if (!(_dictionary.insert({number, {word}}).second)) {
      // Collision detected, then just append an item to the existing vector
      _dictionary[number].push_back(word);
    }
  }
}

std::string::size_type Dictionary::getLongest() const { return _longest; }

std::vector<Word> Dictionary::getWords(std::string number) const {
  return _dictionary.at(number);
}
