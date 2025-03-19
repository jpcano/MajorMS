#include "major.h"

#include <csv.hpp>
#include <fstream>
#include <memory>
#include <stdexcept>

#include "string_number.h"

Major::Major(std::vector<DictionaryConfig> configs) {
  for (auto c : configs) {
    dicts_.push_back(std::make_shared<Dictionary>(c));
  }
}

std::vector<Result> Major::findWords(std::string number, SearchType st) {
  std::vector<Result> ret;

  if (st == SearchType::Separated) {
    for (std::shared_ptr<Dictionary> d : dicts_) {
      auto results = findWords__(number, d, d->getLongest());
      ret.insert(std::end(ret), std::begin(results), std::end(results));
    }
  } else {
    ret = findWords__(number, nullptr, getLongest());
  }
  return ret;
}

std::vector<Word> Major::getWords(std::string number) {
  std::vector<Word> ret = {};
  std::vector<Word> results;
  for (auto d : dicts_) {
    try {
      results = d->getWords(number);
      ret.insert(std::end(ret), std::begin(results), std::end(results));
    } catch (...) {
    }
  }
  if (ret.size() == 0) throw std::out_of_range("No word found");

  return ret;
}

std::string::size_type Major::getLongest() {
  std::string::size_type longest = 0;
  for (auto d : dicts_) {
    if (d->getLongest() > longest) {
      longest = d->getLongest();
    }
  }
  return longest;
}

std::vector<Result> Major::findWords__(std::string number,
                                       std::shared_ptr<Dictionary> dict,
                                       std::string::size_type longest) {
  std::vector<Result> ret;
  for (std::string::size_type i = number.size() / (longest + 1);
       i < number.size(); i++) {
    try {
      auto results = findWords_(number, i, dict, longest);
      if (results.size()) {
        ret.insert(std::end(ret), std::begin(results), std::end(results));
        break;
      }
    } catch (...) {
    }
  }
  return ret;
}

std::vector<Result> Major::findWords_(std::string number, int depth,
                                      std::shared_ptr<Dictionary> dict,
                                      std::string::size_type longest) {
  if (depth == 0) {
    return {{{dict == nullptr ? getWords(number) : dict->getWords(number),
              number}}};
  }

  std::vector<Result> found;
  int a = number.size() - depth * longest;

  for (std::string::size_type i = a > 0 ? a : 0; i < number.size() - depth;
       ++i) {
    try {
      auto n = number.substr(0, i + 1);
      auto current = dict == nullptr ? getWords(n) : dict->getWords(n);
      for (auto i :
           findWords_(number.substr(i + 1), depth - 1, dict, longest)) {
        i.insert(i.begin(), {current, number});
        found.push_back(i);
      }
    } catch (...) {
    }
  }
  return found;
}

void Major::saveWords(std::string out_path, std::string start, std::string end,
                      SearchType st) {
  std::ofstream fs(out_path, std::ofstream::out);

  auto writer = csv::make_csv_writer(fs);

  for (StringNumber n(start); n <= StringNumber(end); n++) {
    auto results = findWords(n.get(), st);
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
