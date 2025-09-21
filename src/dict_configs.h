#pragma once

#include <map>
#include <string>

#include "conversion.h"
#include "dictionary.h"

std::map<std::string, DictConfig> dict_configs = {
    {"es",
     {"es",
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
     {"en",
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
        /*9*/ {"p", "b", "v"}}}}}};

std::string get_dict_names_csv() {
  std::string ret;
  for (const auto& i : dict_configs) ret.append(i.first + ",");
  ret.pop_back();
  return ret;
}
