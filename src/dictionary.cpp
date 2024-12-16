#include "dictionary.h"

#include <utf8.h>

#include <csv.hpp>
#include <fstream>
#include <iostream>
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

    if (!(_dictionary.insert({number, {word}}).second)) {
      // Collision detected, then just append an item to the existing vector
      _dictionary[number].push_back(word);
    }
  }
}

std::vector<Word> Dictionary::getWords(std::string number) {
  try {
    return _dictionary.at(number);
  } catch (std::out_of_range& e) {
    return {};
  }
}

void Dictionary::saveWords(std::string out_path, std::string start,
                           std::string end) {
  long long start_ll = stoll(start);
  long long end_ll = stoll(end);

  if (start_ll > end_ll)
    throw std::invalid_argument(
        "Start number should not be grater than the end number");

  std::ofstream fs(out_path, std::ofstream::out);

  auto writer = csv::make_csv_writer(fs);

  for (long long n = stoll(start); n <= stoll(end); n++) {
    auto words = getWords(std::to_string(n));
    std::string value;

    int i = 0;
    for (auto& word : words) {
      value += word.name + " (" + word.ipa + ")";
      if (i < words.size() - 1) {
        value += ", ";
      }
      i++;
    }

    writer << std::vector<std::string>({std::to_string(n), value});
  }
}
