/// \file test_uri_parser.cpp
/// \brief Unit-tests for uri_parser module.
/// \author Dmitry Kormulev <dmitry.kormulev@yandex,ru>
/// \version 1.0.0.1
/// \date 14.06.2019

#include "uri_parser/uri_parser.h"

#include <string>

#include <gtest/gtest.h>

namespace crypto_wallet {
namespace client {

TEST (UriParserClassTest, GetSchemeValidInput) {
  struct TestSample {
    std::string uri;
    std::string scheme;
  };

  std::vector<TestSample> test_data {
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
  };

  UriParser uri_parser;
  for (const auto &el : test_data) {
    uri_parser.parse(el);
    EXPECT_EQ(uri_parser.get_scheme(), test_data.scheme);
  }
}

TEST (UriParserClassTest, GetSchemeInValidInput) {
  struct TestSample {
    std::string uri;
    std::string scheme;
  };

  std::vector<TestSample> test_data {
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
  };

  UriParser uri_parser;
  for (const auto &el : test_data) {
    uri_parser.parse(el);
    EXPECT_EQ(uri_parser.get_scheme(), test_data.scheme);
  }
}

TEST (UriParserClassTest, GetAuthorityValidInput) {
  struct TestSample {
    std::string uri;
    std::string authority;
  };

  std::vector<TestSample> test_data {
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
  };

  UriParser uri_parser;
  for (const auto &el : test_data) {
    uri_parser.parse(el);
    EXPECT_EQ(uri_parser.get_authority(), test_data.authority);
  }
}

TEST (UriParserClassTest, GetAuthorityInValidInput) {
  struct TestSample {
    std::string uri;
    std::string authority;
  };

  std::vector<TestSample> test_data {
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
  };

  UriParser uri_parser;
  for (const auto &el : test_data) {
    uri_parser.parse(el);
    EXPECT_EQ(uri_parser.get_authority(), test_data.authority);
  }
}

TEST (UriParserClassTest, GetPathValidInput) {
  struct TestSample {
    std::string uri;
    std::string path;
  };

  std::vector<TestSample> test_data {
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
  };

  UriParser uri_parser;
  for (const auto &el : test_data) {
    uri_parser.parse(el);
    EXPECT_EQ(uri_parser.get_path(), test_data.path);
  }
}

TEST (UriParserClassTest, GetPathInValidInput) {
  struct TestSample {
    std::string uri;
    std::string path;
  };

  std::vector<TestSample> test_data {
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
  };

  UriParser uri_parser;
  for (const auto &el : test_data) {
    uri_parser.parse(el);
    EXPECT_EQ(uri_parser.get_path(), test_data.path);
  }
}

TEST (UriParserClassTest, GetQueryValidInput) {
  struct TestSample {
    std::string uri;
    std::string query;
  };

  std::vector<TestSample> test_data {
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
  };

  UriParser uri_parser;
  for (const auto &el : test_data) {
    uri_parser.parse(el);
    EXPECT_EQ(uri_parser.get_query(), test_data.query);
  }
}

TEST (UriParserClassTest, GetQueryInValidInput) {
  struct TestSample {
    std::string uri;
    std::string query;
  };

  std::vector<TestSample> test_data {
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
  };

  UriParser uri_parser;
  for (const auto &el : test_data) {
    uri_parser.parse(el);
    EXPECT_EQ(uri_parser.get_query(), test_data.query);
  }
}

TEST (UriParserClassTest, GetFragmentValidInput) {
  struct TestSample {
    std::string uri;
    std::string fragment;
  };

  std::vector<TestSample> test_data {
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
  };

  UriParser uri_parser;
  for (const auto &el : test_data) {
    uri_parser.parse(el);
    EXPECT_EQ(uri_parser.get_fragment(), test_data.fragment);
  }
}

TEST (UriParserClassTest, GetFragmentInValidInput) {
  struct TestSample {
    std::string uri;
    std::string fragment;
  };

  std::vector<TestSample> test_data {
    {"", ""},
    {"", ""},
    {"", ""},
    {"", ""},
  };

  UriParser uri_parser;
  for (const auto &el : test_data) {
    uri_parser.parse(el);
    EXPECT_EQ(uri_parser.get_fragment(), test_data.fragment);
  }
}

} // namespace client
} // namepsace crypto_wallet

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
