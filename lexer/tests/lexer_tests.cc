#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "lexer/lexer.hh"
#include "lexer/token.hh"

#include <fmt/format.h>

namespace Cougar::Lexer::Tests {

namespace {
Utils::ZoneAllocator zone;

constexpr Token EOF_TOKEN{TokenType::Eof, {}, ""};
} // namespace

MATCHER_P(TokenListEq, expected, "") {

  *result_listener << "\nExpected: "
                   << fmt::format("{{{}}}", fmt::join(expected.begin(),
                                                      expected.end(), ", "))
                   << "\n  Actual: " << fmt::format("{}", arg);

  return std::equal(arg.begin(), arg.end(), expected.begin(), expected.end(),
                    [](const auto &lhs, const auto &rhs) {
                      return std::tie(lhs.type, lhs.content) ==
                             std::tie(rhs.type, rhs.content);
                    });
}

TEST(LexerTest, EmptyStringLiteral) {
  const auto source = R"("")";
  Lexer lex{source};

  const auto result = lex.lex();
  const auto expected = {Token{TokenType::LitString, {}, ""}, EOF_TOKEN};

  EXPECT_THAT(result, TokenListEq(expected));
}

} // namespace Cougar::Lexer::Tests
