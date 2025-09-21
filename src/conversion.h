#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

struct ConversionTable {
  std::vector<std::string> ignore;
  std::vector<std::vector<std::string>> table;
};

class Conversion {
 public:
  Conversion(ConversionTable t);
  std::string phonetic_to_number(std::string phonetic);

 private:
  std::set<std::u32string> _ignore;
  std::map<std::u32string, std::string> _table;
};
