#ifndef MAJOR_H
#define MAJOR_H
#include <memory>
#endif

#include <string>
#include <string_view>
#include <vector>

#include "dictionary.h"

struct Words {
  std::vector<Word> data;
  std::string number;
};

typedef std::vector<Words> Result;

enum SearchType { Merged, Separated };

class Major {
 public:
  Major(std::vector<DictConfig> dicts);
  std::vector<Result> findWords(std::string_view number, SearchType st);
  void saveWords(std::string out_path, std::string start, std::string end,
                 SearchType st);
  static std::string printResults(std::vector<Result> results);

 private:
  std::vector<std::unique_ptr<Dictionary>> dicts_;
  std::vector<Result> findWords__(std::string_view number, int depth,
                                  const std::unique_ptr<Dictionary> &dict,
                                  std::string::size_type longest);
  std::vector<Result> findWords_(std::string_view number,
                                 const std::unique_ptr<Dictionary> &dict,
                                 std::string::size_type longest);
  std::string::size_type getLongest();
  std::vector<Word> getWords(std::string_view number);
};
