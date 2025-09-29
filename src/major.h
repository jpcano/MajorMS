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
  std::vector<Result> findWords(std::string_view number, SearchType st,
                                unsigned int splits);
  void saveWords(std::string out_path, std::string start, std::string end,
                 SearchType st, unsigned int splits);
  static std::string printResults(std::vector<Result> results);

 private:
  using GetWordsCallback = std::function<std::vector<Word>(std::string_view)>;

  std::vector<std::unique_ptr<Dictionary>> dicts_;
  std::vector<Result> findWords_(std::string_view number,
                                 std::string::size_type longest,
                                 GetWordsCallback gw, unsigned int splits);
  std::vector<Result> findWords__(std::string_view number, int depth,

                                  std::string::size_type longest,
                                  GetWordsCallback gw);

  std::vector<Word> getWords(std::string_view number);
  std::size_t longest_ = 0;
};
