#include <utf8.h>

#include <map>
#include <nlohmann/json.hpp>
#include <set>
#include <string>

using json = nlohmann::json;

class Phonemes {
 public:
  Phonemes(std::string config_path);
  void printConfig();
  std::string getNumber(utf8::utfchar32_t ch);

 private:
  std::string _config_path;
  json _config;
  std::set<utf8::utfchar32_t> _ignore;
  std::map<utf8::utfchar32_t, std::string> _table;
  bool isIgnore(utf8::utfchar32_t ch);
};
