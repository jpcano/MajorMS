#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <cereal/access.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <map>
#include <string>
#include <vector>

#include "conversion.h"

struct DictConfig {
  std::string name;
  std::string short_name;
  std::string dictionary_path;
  ConversionTable table;

  template <class Archive>
  void serialize(Archive& ar, std::uint32_t archiveVersion) {
    ar(CEREAL_NVP(name), CEREAL_NVP(short_name), CEREAL_NVP(dictionary_path),
       CEREAL_NVP(table));
  }
};
CEREAL_CLASS_VERSION(DictConfig, 1);

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
  Dictionary(DictConfig config);
  std::vector<Word> getWords(std::string number) const;
  std::string::size_type getLongest() const;

 private:
  std::string::size_type _longest = 0;
  std::map<std::string, std::vector<Word>> _dictionary;

  friend class cereal::access;
  Dictionary() {};
  template <class Archive>
  void serialize(Archive& ar, std::uint32_t archiveVersion) {
    ar(_longest, _dictionary);
  }
};

CEREAL_CLASS_VERSION(Dictionary, 1);
#endif
