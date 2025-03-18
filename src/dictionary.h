#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <map>
#include <string>
#include <vector>

#include "phonemes.h"

struct DictionaryConfig {
  std::string name;
  std::string phonemes_path;
  std::string dictionary_path;
};

struct Word {
  std::string name;
  std::string ipa;
};

class Dictionary {
 public:
  Dictionary(DictionaryConfig config);
  std::vector<Word> getWords(std::string number);
  std::string::size_type getLongest() const;

 private:
  std::string::size_type _longest = 0;
  Phonemes _phonemes;
  DictionaryConfig _config;
  std::map<std::string, std::vector<Word>> _dictionary;
};

#endif
