#ifndef STRING_NUMBER_H
#define STRING_NUMBER_H

#include <string>

class StringNumber {
 public:
  StringNumber(std::string number);
  StringNumber& operator++();
  StringNumber operator++(int);
  bool operator<(const StringNumber& rhs);
  bool operator>(const StringNumber& rhs);
  bool operator<=(const StringNumber& rhs);
  bool operator>=(const StringNumber& rhs);
  bool operator==(const StringNumber& rhs);
  bool operator!=(const StringNumber& rhs);
  std::string get() const;

 private:
  std::string number_;
};
#endif
