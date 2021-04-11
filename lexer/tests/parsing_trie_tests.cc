#include <gtest/gtest.h>

#include "lexer/parsing_trie.hh"

namespace Cougar::Lexer::Tests {

enum TOKEN {
  TOKEN_PLUS,
  TOKEN_PLUS_PLUS,
  TOKEN_PLUS_EQUAL,
  TOKEN_EUQAL,
  TOKEN_COMMA,
  TOKEN_SHIFT_LEFT
};

using PT = PatternToken<std::string, TOKEN>;

const PT tokens[] = {
    //
    {"+", TOKEN_PLUS},        //
    {"++", TOKEN_PLUS_PLUS},  //
    {"+=", TOKEN_PLUS_EQUAL}, //
    {"=", TOKEN_EUQAL},       //
    {",", TOKEN_COMMA},       //
    {"<<", TOKEN_SHIFT_LEFT}, //

};

struct CountingIterator {
  const char *mData;
  unsigned &pos;

  char operator*() const { return *mData; }
  void operator++() {
    ++mData;
    ++pos;
  }
};

TEST(ParsingTrieTest, BasicTest) {

  ParsingTrie<char, TOKEN> trie;
  trie.build(std::begin(tokens), std::end(tokens));

  struct Expectation {
    const char *input;
    unsigned expectedPos;
    TOKEN expectedToken;
  };

  const Expectation expectations[] = {
      //
      {"+", 1, TOKEN_PLUS},        //
      {"++", 2, TOKEN_PLUS_PLUS},  //
      {"+=", 2, TOKEN_PLUS_EQUAL}, //
      {"++=", 2, TOKEN_PLUS_PLUS}, //
      {"=+", 1, TOKEN_EUQAL},      //
      {"+?", 1, TOKEN_PLUS},       //
  };

  for (const Expectation &e : expectations) {
    unsigned pos = 0;
    CountingIterator it{e.input, pos};
    auto maybeToken = trie.find(it);
    ASSERT_TRUE(maybeToken);
    EXPECT_EQ(e.expectedToken, *maybeToken) << "when parsing " << e.input;
    EXPECT_EQ(e.expectedPos, pos) << "when parsing " << e.input;
  }
}

} // namespace Cougar::Lexer::Tests