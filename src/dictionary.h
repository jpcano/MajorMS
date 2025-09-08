#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <cereal/access.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <map>
#include <string>
#include <vector>

#include "phonemes.h"

struct DictionaryConfig {
  std::string name;
  std::string phonemes_path;
  std::string dictionary_path;

 private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive& ar) {
    ar(name, phonemes_path, dictionary_path);
  }
};

struct Word {
  std::string lang;
  std::string name;
  std::string ipa;

 private:
  friend class cereal::access;
  template <class Archive>
  void serialize(Archive& ar) {
    ar(lang, name, ipa);
  }
};

class Dictionary {
 public:
  Dictionary(DictionaryConfig config);
  std::vector<Word> getWords(std::string number) const;
  std::string::size_type getLongest() const;

 private:
  std::string::size_type _longest = 0;
  DictionaryConfig _config;
  std::map<std::string, std::vector<Word>> _dictionary;

  friend class cereal::access;
  Dictionary() {};
  template <class Archive>
  void serialize(Archive& ar) {
    ar(_longest, _config, _dictionary);
  }
};

CEREAL_CLASS_VERSION(Dictionary, 1);
#endif
