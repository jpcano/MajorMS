#include "dictionary.h"

#include <utf8.h>

#include <fstream>
#include <iostream>
#include <sstream>

Dictionary::Dictionary(std::string phonemes_path, std::string dictionary_path)
    : _phonemes(phonemes_path), _dictionary_path(dictionary_path) {
  std::ifstream file(_dictionary_path);
  std::string line;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    Word word;
    std::getline(iss, word.name, '\t');
    std::getline(iss, word.ipa, '\t');
    word.ipa.erase(0, 1);
    word.ipa.erase(word.ipa.size() - 1);
    std::string::iterator it = word.ipa.begin();
    int ipa_len = utf8::distance(word.ipa.begin(), word.ipa.end());
    std::string number;
    for (int i = 0; i < ipa_len; i++) {
      utf8::utfchar32_t ch = utf8::next(it, word.ipa.end());
      if (!_phonemes.isIgnore(ch)) {
        number += _phonemes.getNumber(ch);
      }
    }
    if (!(_dictionary.insert({number, {word}}).second)) {
      _dictionary[number].push_back(word);
    }
  }
  for (const auto &[key, value] : _dictionary) {
    std::cout << "key: " << key << std::endl;
  }
}

std::vector<Word> Dictionary::getWords(std::string number) {
  return _dictionary.at(number);
}
