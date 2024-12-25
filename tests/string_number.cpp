#include "string_number.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

TEST(StringNumber, constructor) {
  ASSERT_THROW(StringNumber a("");, std::exception);
  ASSERT_THROW(StringNumber a("a");, std::exception);
  ASSERT_THROW(StringNumber a("1a2");, std::exception);
  ASSERT_THROW(StringNumber a("-1");, std::exception);
  ASSERT_THROW(StringNumber a("+2");, std::exception);
  ASSERT_NO_THROW(StringNumber a("0"););
  ASSERT_NO_THROW(StringNumber a("1"););
  ASSERT_NO_THROW(StringNumber a("2"););
  ASSERT_NO_THROW(StringNumber a("123"););
}

TEST(StringNumber, increment) {
  StringNumber n("0");
  n++;
  ASSERT_THAT(n.get(), testing::StrEq("1"));
  ++n;
  ASSERT_THAT(n.get(), testing::StrEq("2"));

  StringNumber o("98");
  o++;
  ASSERT_THAT(o.get(), testing::StrEq("99"));
  o++;
  ASSERT_THAT(o.get(), testing::StrEq("100"));
}

TEST(StringNumber, comparison) {
  StringNumber n1("0");
  StringNumber n2("0");
  ASSERT_TRUE(n1 == n2++);
  ASSERT_TRUE(n1 != n2);
  ASSERT_TRUE(n1 < n2);
  ASSERT_TRUE(n1 <= n2);
  ASSERT_FALSE(n1 > n2);
  ASSERT_FALSE(n1 >= n2);
  ASSERT_TRUE(++n1 == n2);
  ASSERT_TRUE(n1 >= n2);
  ASSERT_TRUE(n1 <= n2);
}
