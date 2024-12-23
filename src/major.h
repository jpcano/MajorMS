#ifndef MAJOR_H
#define MAJOR_H
#endif

#include <string>
#include <vector>

#include "dictionary.h"

struct Words {
  std::vector<Word> data;
  std::string number;
};

typedef std::vector<Words> Result;

class Major {
 public:
  Major(std::string phonemes_path, std::string dictionary_path);
  std::vector<Result> findWords(std::string number);
  void saveWords(std::string out_path, std::string start, std::string end);

 private:
  Dictionary dict_;
  std::vector<Result> findWords_(std::string number, int depth);
};
