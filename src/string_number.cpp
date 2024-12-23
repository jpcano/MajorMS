#include "string_number.h"

#include <iostream>
#include <string>

StringNumber::StringNumber(std::string number) {
  for (std::string::size_type i = 0; i < number.size(); ++i) {
    stoi(number.substr(i, 1));
  }
  number_ = number;
}

StringNumber& StringNumber::operator++() {
  std::string new_number;
  int digit;
  bool carry = true;

  for (int i = number_.size() - 1; i >= 0; --i) {
    if (carry) {
      digit = stoi(number_.substr(i, 1));
      if (++digit == 10)
        digit = 0;
      else
        carry = false;
      new_number.insert(0, std::to_string(digit));
    } else
      new_number.insert(0, number_.substr(i, 1));
  }
  if (carry) new_number.insert(0, "1");
  number_ = new_number;
  return *this;
}

StringNumber StringNumber::operator++(int) {
  StringNumber old = *this;  // copy old value
  operator++();              // prefix increment
  return old;                // return old value
}

bool StringNumber::operator<(const StringNumber& rhs) {
  std::string ln = get();
  std::string rn = rhs.get();
  if (ln.size() == rn.size()) {
    for (std::string::size_type i = 0; i < ln.size(); ++i) {
      if (ln[i] == rn[i])
        continue;
      else {
        return ln[i] < rn[i];
      }
    }
    return false;
  } else
    return ln.size() < rn.size();
}

bool StringNumber::operator>(const StringNumber& rhs) {
  return !(*this <= rhs);
}

bool StringNumber::operator<=(const StringNumber& rhs) {
  return *this < rhs || *this == rhs;
}

bool StringNumber::operator>=(const StringNumber& rhs) {
  return !(*this < rhs);
}

bool StringNumber::operator==(const StringNumber& rhs) {
  return get() == rhs.get();
}

bool StringNumber::operator!=(const StringNumber& rhs) {
  return get() != rhs.get();
}

std::string StringNumber::get() const { return number_; };
