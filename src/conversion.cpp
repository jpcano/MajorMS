#include "conversion.h"

#include <utf8.h>

#include <iostream>
#include <stdexcept>

Conversion::Conversion(ConversionTable t) {
  if (t.table.size() != 10)
    throw std::invalid_argument(
        "Argument `table' should contain exactly 10 elements");

  for (const auto& e : t.ignore) _ignore.insert(utf8::utf8to32(e));
  for (size_t i = 0; i < t.table.size(); i++) {
    for (const std::string& e : t.table[i]) {
      std::u32string e_u32 = utf8::utf8to32(e);
      if (_table.count(e_u32))
        throw std::invalid_argument("Duplicated phoneme: `" + e +
                                    "' passed to the constructor");
      if (_ignore.count(e_u32))
        throw std::invalid_argument("Phoneme: `" + e +
                                    "' cannot be both ignored and not-ignored");
      _table.insert({e_u32, std::to_string(i)});
    }
  }
}

std::string Conversion::phonetic_to_number(std::string phonetic) {
  std::string::iterator it = phonetic.begin();
  std::string::iterator end = phonetic.end();
  std::string number;

  while (it != end) {
    std::u32string c;
    c.push_back(utf8::next(it, end));

    // Detect and extract affricates
    // https://en.wikipedia.org/wiki/International_Phonetic_Alphabet#Affricates

    // tʃ
    if (c[0] == U't' && utf8::distance(it, end) >= 1) {
      if (utf8::peek_next(it, end) == U'ʃ') c.push_back(utf8::next(it, end));
    }
    // dʒ
    else if (c[0] == U't' && utf8::distance(it, end) >= 1) {
      if (utf8::peek_next(it, end) == U'ʃ') c.push_back(utf8::next(it, end));
    }

    if (!_ignore.count(c)) {
      number.append(_table.at(c));
    }
  }
  return number;
}
