#include "major.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/memory.hpp>
#include <csv.hpp>
#include <fstream>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "string_number.h"

std::map<Dicts, DictionaryConfig> dicts_configuration = {
    {Dicts::ES, {"es", "../data/config_es_ES.json", "../data/es_ES.txt"}},
    {Dicts::EN, {"en", "../data/config_en_UK.json", "../data/en_UK.txt"}}};

Major::Major(std::vector<Dicts> dicts) {
  for (auto dict : dicts) {
    DictionaryConfig dictConfig = dicts_configuration[dict];
    std::unique_ptr<Dictionary> d;
    std::ifstream iFile(dictConfig.dictionary_path + ".cereal");
    if (iFile.good()) {
      cereal::BinaryInputArchive iArchive(iFile);
      iArchive(d);
    } else {
      d = std::make_unique<Dictionary>(dictConfig);
      std::ofstream oFile(dictConfig.dictionary_path + ".cereal");
      cereal::BinaryOutputArchive oArchive(oFile);
      oArchive(d);
    }
    dicts_.push_back(std::move(d));
  }
}

std::vector<Result> Major::findWords(std::string number, SearchType st) {
  std::vector<Result> ret;

  if (st == SearchType::Separated) {
    for (const auto& d : dicts_) {
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
  for (const auto& d : dicts_) {
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
  for (const auto& d : dicts_) {
    if (d->getLongest() > longest) {
      longest = d->getLongest();
    }
  }
  return longest;
}

std::vector<Result> Major::findWords__(std::string number,
                                       const std::unique_ptr<Dictionary>& dict,
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
                                      const std::unique_ptr<Dictionary>& dict,
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
    std::string value = Major::printResults(results);
    writer << std::vector<std::string>({n.get(), value});
  }
}

std::string Major::printResults(std::vector<Result> results) {
  std::stringstream ss;
  int i = 0, j = 0;

  for (auto& result : results) {
    for (auto& words : result) {
      i = 0;
      for (auto& word : words.data) {
        ss << word.name << " (" << word.ipa << " | " << word.lang << ")";
        if (i++ < words.data.size() - 1) ss << ", ";
      }
      ss << std::endl << std::endl;
    }
    if ((results.size() - 1) != j++) ss << "---" << std::endl << std::endl;
  }

  return ss.str();
}
