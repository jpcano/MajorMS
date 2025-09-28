#pragma once

#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <map>
#include <set>
#include <string>
#include <string_view>
#include <vector>

struct ConversionTable {
  std::vector<std::string> ignore;
  std::vector<std::vector<std::string>> table;

  template <class Archive>
  void serialize(Archive& ar, std::uint32_t archiveVersion) {
    ar(CEREAL_NVP(ignore), CEREAL_NVP(table));
  }
};
CEREAL_CLASS_VERSION(ConversionTable, 1);

class Conversion {
 public:
  Conversion(ConversionTable t);
  std::string phonetic_to_number(std::string_view phonetic);

 private:
  std::set<std::u32string> _ignore;
  std::map<std::u32string, std::string> _table;
};
