#include "dictionary.h"

#include <utf8.h>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

Dictionary::Dictionary(std::string phonemes_path, std::string dictionary_path)
    : _phonemes(phonemes_path), _dictionary_path(dictionary_path) {
  std::ifstream file(_dictionary_path);
  std::string line;

  while (std::getline(file, line)) {
    std::istringstream iss(line);
    Word word;
    std::getline(iss, word.name, '\t');
    std::getline(iss, word.ipa, '\t');

    // Erase the '/' characters that surround the IPA pronunciation
    word.ipa.erase(0, 1);
    word.ipa.erase(word.ipa.size() - 1);

    std::string::iterator it = word.ipa.begin();
    int ipa_len = utf8::distance(word.ipa.begin(), word.ipa.end());
    std::string number;

    for (int i = 0; i < ipa_len; i++) {
      std::vector<utf8::utfchar32_t> ch;
      int len = _phonemes.nextPhoneme(it, word.ipa.end(), ch);
      i += len - 1;
      if (!_phonemes.isIgnore(ch)) {
        number += _phonemes.getNumber(ch);
      }
    }

    _longest = number.size() > _longest ? number.size() : _longest;

    if (!(_dictionary.insert({number, {word}}).second)) {
      // Collision detected, then just append an item to the existing vector
      _dictionary[number].push_back(word);
    }
  }
}

std::string::size_type Dictionary::getLongest() const { return _longest; }

std::vector<Word> Dictionary::getWords(std::string number) {
  return _dictionary.at(number);
}
