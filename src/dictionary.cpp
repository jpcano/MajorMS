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
    word.ipa.erase(0, 1);
    word.ipa.erase(word.ipa.size() - 1);
    std::string::iterator it = word.ipa.begin();
    int ipa_len = utf8::distance(word.ipa.begin(), word.ipa.end());
    std::string number;
    //    std::cout << "heyword: " << word.ipa << std::endl;
    for (int i = 0; i < ipa_len; i++) {
      utf8::utfchar32_t c = utf8::next(it, word.ipa.end());
      std::vector<utf8::utfchar32_t> ch;
      ch.push_back(c);
      if (c == 116) {
        try {
          if (utf8::peek_next(it, word.ipa.end()) == 643) {
            //            std::cout << "hey: " << word.ipa << std::endl;
            ch.push_back(utf8::next(it, word.ipa.end()));
            i++;
          }
        } catch (utf8::not_enough_room& e) {
        }
      }
      //      std::cout << "later" << std::endl;
      if (!_phonemes.isIgnore(ch)) {
        number += _phonemes.getNumber(ch);
      }
    }
    if (!(_dictionary.insert({number, {word}}).second)) {
      _dictionary[number].push_back(word);
    }
  }
  // for (const auto &[key, value] : _dictionary) {
  //   std::cout << "key: " << key << std::endl;
  // }
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
  std::ofstream fs(out_path, std::ofstream::out);

  auto writer = csv::make_csv_writer(fs);

  for (int n = stoi(start); n <= stoi(end); n++) {
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
