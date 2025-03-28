#ifndef MAJOR_H
#define MAJOR_H
#include <memory>
#endif

#include <string>
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
  Major(std::vector<DictionaryConfig> configs);
  std::vector<Result> findWords(std::string number, SearchType st);
  void saveWords(std::string out_path, std::string start, std::string end,
                 SearchType st);

 private:
  std::vector<std::shared_ptr<Dictionary>> dicts_;
  std::vector<Result> findWords_(std::string number, int depth,
                                 std::shared_ptr<Dictionary> dict,
                                 std::string::size_type longest);
  std::vector<Result> findWords__(std::string number,
                                  std::shared_ptr<Dictionary> dict,
                                  std::string::size_type longest);
  std::string::size_type getLongest();
  std::vector<Word> getWords(std::string number);
};
