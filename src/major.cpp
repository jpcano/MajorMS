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

Major::Major(std::vector<DictConfig> dicts) {
  for (auto dict : dicts) {
    // std::unique_ptr<Dictionary> d = std::make_unique<Dictionary>(dict);
    std::unique_ptr<Dictionary> d;
    std::ifstream iFile(dict.dictionary_path + ".cereal");
    if (iFile.good()) {
      cereal::BinaryInputArchive iArchive(iFile);
      iArchive(d);
    } else {
      d = std::make_unique<Dictionary>(dict);
      std::ofstream oFile(dict.dictionary_path + ".cereal");
      cereal::BinaryOutputArchive oArchive(oFile);
      oArchive(d);
    }
    dicts_.push_back(std::move(d));
  }

  for (const auto& d : dicts_) {
    if (d->getLongest() > longest_) {
      longest_ = d->getLongest();
    }
  }
}

std::vector<Word> Major::getWords(std::string_view number) {
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

std::vector<Result> Major::findWords(std::string_view number, SearchType st) {
  std::vector<Result> ret;

  if (st == SearchType::Separated) {
    for (const auto& dict : dicts_) {
      std::vector<Result> results =
          findWords_(number, dict, dict->getLongest());
      ret.insert(std::end(ret), std::begin(results), std::end(results));
    }
  } else {
    ret = findWords_(number, nullptr, longest_);
  }
  return ret;
}

std::vector<Result> Major::findWords_(std::string_view number,
                                      const std::unique_ptr<Dictionary>& dict,
                                      std::string::size_type longest) {
  std::vector<Result> ret;
  for (std::string::size_type depth = number.size() / (longest + 1);
       depth < number.size(); depth++) {
    try {
      ret = findWords__(number, depth, dict, longest);
      if (ret.size()) break;
    } catch (...) {
    }
  }
  return ret;
}

std::vector<Result> Major::findWords__(std::string_view number, int depth,
                                       const std::unique_ptr<Dictionary>& dict,
                                       std::string::size_type longest) {
  if (depth == 0) {
    Words words;

    if (dict == nullptr)
      words = {getWords(number), static_cast<std::string>(number)};
    else
      words = {dict->getWords(number), static_cast<std::string>(number)};

    return std::vector({std::vector({words})});
  }

  std::vector<Result> found;

  for (std::string::size_type i = 0; i < number.size() - depth; ++i) {
    try {
      std::string_view number_left = number.substr(0, i + 1);
      std::string_view number_right = number.substr(i + 1);
      std::vector<Word> current;

      if (dict == nullptr)
        current = getWords(number_left);
      else
        current = dict->getWords(number_left);

      for (Result& partial_result :
           findWords__(number_right, depth - 1, dict, longest)) {
        partial_result.insert(partial_result.begin(),
                              Words{current, static_cast<std::string>(number)});
        found.push_back(partial_result);
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
