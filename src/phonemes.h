#ifndef PHONEMES_H
#define PHONEMES_H

#include <utf8.h>
#include <xxhash.h>

#include <map>
#include <nlohmann/json.hpp>
#include <set>
#include <string>
#include <vector>

using json = nlohmann::json;

class Phonemes {
 public:
  Phonemes(std::string config_path);
  void printConfig();
  std::string getNumber(std::vector<utf8::utfchar32_t> ch, std::string word,
                        std::string ipa);
  bool isIgnore(std::vector<utf8::utfchar32_t> ch);
  int nextPhoneme(std::string::iterator &it, std::string::iterator end,
                  std::vector<utf8::utfchar32_t> &ch);

 private:
  std::string _config_path;
  XXH128_hash_t _config_hash;
  json _config;
  std::set<std::vector<utf8::utfchar32_t>> _ignore;
  std::map<std::vector<utf8::utfchar32_t>, std::string> _table;
};

#endif
