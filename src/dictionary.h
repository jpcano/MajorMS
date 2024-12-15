#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <map>
#include <string>
#include <vector>

#include "phonemes.h"

struct Word {
  std::string name;
  std::string ipa;
};

class Dictionary {
 public:
  Dictionary(std::string phonemes_path, std::string dictionary_path);
  std::vector<Word> getWords(std::string number);

 private:
  std::string _dictionary_path;
  Phonemes _phonemes;
  std::map<std::string, std::vector<Word>> _dictionary;
};

#endif
