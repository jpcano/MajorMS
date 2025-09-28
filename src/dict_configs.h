#pragma once

#include <cereal/access.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/string.hpp>
#include <fstream>
#include <map>
#include <sstream>
#include <string>

#include "conversion.h"
#include "dictionary.h"

struct DictConfigs {
  std::map<std::string, DictConfig> configs;

  template <class Archive>
  void serialize(Archive& ar, std::uint32_t archiveVersion) {
    ar(CEREAL_NVP(configs));
  }
};
CEREAL_CLASS_VERSION(DictConfigs, 1);

DictConfigs dict_configs = {
    {{"es",
      {"Spanish <https://github.com/open-dict-data/ipa-dict>",
       "es",
       "../data/es_ES.txt",
       {{"a", "e", "i", "o", "u", "j", "w", "ˈ", ".", "/", ",", " ", "'"},
        {/*0*/ {"s", "θ", "z"},
         /*1*/ {"t", "d", "ð"},
         /*2*/ {"n", "ɲ", "ŋ", "ɱ"},
         /*3*/ {"m"},
         /*4*/ {"r", "ɾ"},
         /*5*/ {"l", "ʎ", "ʝ"},
         /*6*/ {"ʃ", "tʃ"},
         /*7*/ {"g", "ɣ", "k", "x"},
         /*8*/ {"f"},
         /*9*/ {"p", "b", "β"}}}}},
     {"en",
      {"English (UK) <https://github.com/open-dict-data/ipa-dict>",
       "en",
       "../data/en_UK.txt",
       {{"a", "e", "i", "o", "u", "j", "w", "ˈ", ",", " ", "ʊ", "ə", "ɔ", "ɪ",
         "ɛ", "ˌ", "ɑ", "ɝ", "ː", "ɐ", "æ", "ɒ", "ʌ", "ɜ", "ʲ", "ɑ̃", "̃"},
        {/*0*/ {"s", "θ", "z"},
         /*1*/ {"t", "d", "ð"},
         /*2*/ {"n", "ŋ", "ɱ"},
         /*3*/ {"m"},
         /*4*/ {"r", "ɹ"},
         /*5*/ {"l", "ʒ", "ɬ"},
         /*6*/ {"ʃ", "tʃ", "dʒ"},
         /*7*/ {"k", "x", "ɡ", "h"},
         /*8*/ {"f"},
         /*9*/ {"p", "b", "v"}}}}}}};

std::string get_dict_names_csv() {
  std::string ret;
  for (const auto& i : dict_configs.configs) ret.append(i.first + ",");
  ret.pop_back();
  return ret;
}

std::string get_conversion_tables() {
  std::ostringstream ret;

  for (const auto& c : dict_configs.configs) {
    int number_of_lines = 0;
    std::string line;
    std::ifstream myfile(c.second.dictionary_path);
    while (std::getline(myfile, line)) ++number_of_lines;

    ret << "[" << c.first << "]" << std::endl;
    ret << "\t" << "Name: " << c.second.name << std::endl;
    ret << "\t" << "Words: " << number_of_lines << std::endl;
    ret << "\t" << "Path: " << c.second.dictionary_path << std::endl;
    ret << "\t" << "Ignored: ";
    for (const auto& ignore : c.second.table.ignore) {
      ret << ignore << " ";
    }
    ret << std::endl;
    for (size_t i = 0; i < c.second.table.table.size(); i++) {
      ret << "\t" << std::to_string(i) << ": ";
      for (const auto& c : c.second.table.table[i]) {
        ret << c << " ";
      }
      ret << std::endl;
    }
  }
  return ret.str();
}
