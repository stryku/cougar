#include "lexer_adaptor.hh"

#include "utils/file_loader.hh"

#include "lexer/lexer.hh"

#include <cassert>

namespace Cougar::Parser {

thread_local LexerAdaptor *tlCurrentInstance = nullptr;

int LexerAdaptor::lex() {
  tlCurrentInstance->mLastToken = *tlCurrentInstance->mCurrent;
  if (tlCurrentInstance->mLastToken->type == Lexer::TokenType::Eof)
    return -1;

  tlCurrentInstance->mCurrent++;
  return int(tlCurrentInstance->mLastToken->type);
}

std::string_view LexerAdaptor::currentText() {
  return tlCurrentInstance->mLastToken->content;
}

LexerAdaptor::LexerAdaptor(Utils::ZoneAllocator &zone) : mZone(zone) {
  assert(!tlCurrentInstance);
  tlCurrentInstance = this;
}

LexerAdaptor::~LexerAdaptor() {
  assert(tlCurrentInstance);
  tlCurrentInstance = nullptr;
}

void LexerAdaptor::loadFile(const std::string &path) {
  Utils::FileLoader loader;
  loader.load(path);

  std::byte *buffer = mZone.allocateBlock(loader.getSize());
  loader.copyTo(buffer);

  std::string_view sv((const char *)buffer, loader.getSize());

  mTokens = Lexer::lexBuffer(sv, mZone);
  mCurrent = mTokens.data();
}

} // namespace Cougar::Parser