#include <dict_configs.h>
#include <gtest/gtest.h>
#include <major.h>

#include <filesystem>
#include <fstream>
#include <iostream>

DictConfig create_test_dict(const std::string &key, const std::string &text) {
  std::string dir_path = "../data/" + key + ".test.txt";
  std::ofstream file;
  std::filesystem::remove(dir_path + ".cereal");
  file.open(dir_path);
  file << text;
  DictConfig dc = dict_configs.configs.at(key);
  dc.dictionary_path = dir_path;
  return dc;
}

void test_find(Major &major, const std::string &query,
               const std::string &expected,
               SearchType search_type = SearchType::Separated) {
  std::vector<Result> ret = major.findWords(query, search_type);
  std::string result = major.printResults(ret);
  EXPECT_STREQ(result.c_str(), expected.c_str());
}

TEST(Major, OneWordIsAllIgnore) {
  std::string raw_dict =
      "ada\t/ada/\n"
      "aeiou\t/aeiou/\n"
      "amo\t/amo/\n";
  Major major({create_test_dict("es", raw_dict)});

  test_find(major, "", "");
  test_find(major, "42", "");
}

TEST(Major, BasicWords) {
  std::string raw_dict =
      "teta\t/teta/\n"
      "pepe\t/pepe/\n"
      "lastase\t/lastase/\n"
      "evidenciara\t/eβiðenθjaɾa/\n"
      "degollé\t/deɣoˈʎe/\n";
  Major major({create_test_dict("es", raw_dict)});

  test_find(major, "11", "teta (teta | es)\n\n");
  test_find(major, "99", "pepe (pepe | es)\n\n");
  test_find(major, "91204", "evidenciara (eβiðenθjaɾa | es)\n\n");
}

TEST(Major, MultiWordResults) {
  std::string raw_dict =
      "teta\t/teta/\n"
      "pepe\t/pepe/\n"
      "lastase\t/lastase/\n"
      "popo\t/popo/\n";
  Major major({create_test_dict("es", raw_dict)});

  test_find(major, "99", "pepe (pepe | es), popo (popo | es)\n\n");
}

TEST(Major, Affricates) {
  std::string raw_dict_es = "delgaducha\t/delgaðutʃa/\n";
  Major major_es({create_test_dict("es", raw_dict_es)});

  test_find(major_es, "15716", "delgaducha (delgaðutʃa | es)\n\n");

  std::string raw_dict_en = "abjection\t/æbdʒˈɛkʃən/\n";
  Major major_en({create_test_dict("en", raw_dict_en)});

  test_find(major_en, "96762", "abjection (æbdʒˈɛkʃən | en)\n\n");
}

TEST(Major, MultipleDicts) {
  std::string raw_dict = "affricateword\t/attʃt/\n";
  Major major(
      {create_test_dict("es", raw_dict), create_test_dict("en", raw_dict)});

  test_find(major, "", "");
  test_find(major, "42", "");
  test_find(
      major, "161",
      "affricateword (attʃt | es)\n\n---\n\naffricateword (attʃt | en)\n\n");
}