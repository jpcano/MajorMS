#include "major.h"

#include <csv.hpp>
#include <fstream>

#include "string_number.h"

Major::Major(std::string phonemes_path, std::string dictionary_path)
    : dict_(phonemes_path, dictionary_path) {}

std::vector<Result> Major::findWords(std::string number) {
  for (std::string::size_type i = number.size() / (dict_.getLongest() + 1);
       i < number.size(); i++) {
    try {
      auto results = findWords_(number, i);
      if (results.size()) return results;
    } catch (...) {
    }
  }
  return {};
}

std::vector<Result> Major::findWords_(std::string number, int depth) {
  if (depth == 0) {
    return {{{dict_.getWords(number), number}}};
  }

  std::vector<Result> found;
  int a = number.size() - depth * dict_.getLongest();

  for (std::string::size_type i = a > 0 ? a : 0; i < number.size() - depth;
       ++i) {
    try {
      std::vector<Word> current = dict_.getWords(number.substr(0, i + 1));
      for (auto i : findWords_(number.substr(i + 1), depth - 1)) {
        i.insert(i.begin(), {current, number});
        found.push_back(i);
      }
    } catch (...) {
    }
  }
  return found;
}

void Major::saveWords(std::string out_path, std::string start,
                      std::string end) {
  std::ofstream fs(out_path, std::ofstream::out);

  auto writer = csv::make_csv_writer(fs);

  for (StringNumber n(start); n <= StringNumber(end); n++) {
    auto results = findWords(n.get());
    std::string value;

    int j = 0;
    for (auto& result : results) {
      for (auto& words : result) {
        int i = 0;
        for (auto& word : words.data) {
          value += word.name + " (" + word.ipa + ")";
          if (i++ < words.data.size() - 1) value += ", ";
        }
        value += "\n";
      }
      if ((results.size() - 1) != j++) value += "---\n";
    }

    writer << std::vector<std::string>({n.get(), value});
  }
}
