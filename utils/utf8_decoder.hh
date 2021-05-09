#pragma once

#include <cstdint>
#include <stdexcept>
#include <string_view>

namespace Cougar::Utils {

using rune_t = std::uint32_t;

// Moves over UTF-8 bufffer, extracting "runes" (term borrowed from Golang),
// a USC4 characters, or 0 at the end.
// Also normalizes endlines to '\n'
class Utf8Decoder {
public:
  Utf8Decoder(std::string_view utf8Buffer) {
    mCurrent = reinterpret_cast<const std::uint8_t *>(utf8Buffer.data());
    mEnd = reinterpret_cast<const std::uint8_t *>(utf8Buffer.end());

    // TODO detect and skip over BOM here

    mNext = doGetNext();
  }

  const char *getCurrentPosition() const {
    return reinterpret_cast<const char *>(mPrevCurrent);
  }

  rune_t next() {
    rune_t n = mNext;
    mNext = doGetNext();
    return n;
  }

  rune_t peekNext() const { return mNext; }

private:
  rune_t doGetNext() {
    mPrevCurrent = mCurrent;
    if (mCurrent == mEnd)
      return 0;

    // turn '\r\n' into '\n'
    if (*mCurrent == '\r') {
      mCurrent++;
      if (mCurrent == mEnd)
        return '\n';
      if (*mCurrent == '\n') {
        mCurrent++;
        return '\n';
      }
    }

    // see https://en.wikipedia.org/wiki/UTF-8#Encoding

    // ascii char
    if ((*mCurrent & 0x80) == 0) {
      return rune_t(*mCurrent++);
    }

    // 2-byte char
    if ((*mCurrent & 0xE0) == 0xC0) {
      rune_t rune = *mCurrent & 0x1F;
      mCurrent++;
      std::uint8_t byte2 = getContinuation();
      rune <<= 6;
      rune += byte2 & 0x3F;
      return rune;
    }

    // 3 byte char
    if ((*mCurrent & 0xF0) == 0xE0) {
      rune_t rune = *mCurrent & 0x0F;
      mCurrent++;
      std::uint8_t byte2 = getContinuation();
      std::uint8_t byte3 = getContinuation();
      rune <<= 12;
      rune += (byte2 << 6);
      rune += byte3;
      return rune;
    }

    // 4-byte char
    if ((*mCurrent & 0xF8) == 0xF0) {
      rune_t rune = *mCurrent & 0x7;
      mCurrent++;
      std::uint8_t byte2 = getContinuation();
      std::uint8_t byte3 = getContinuation();
      std::uint8_t byte4 = getContinuation();
      rune <<= 18;
      rune += (byte2 << 12);
      rune += (byte3 << 6);
      rune += byte4;
      return rune;
    }

    throw std::runtime_error("Invalid UTF-8 sequence");
  }

  std::uint8_t getContinuation() {
    if (mCurrent == mEnd)
      throw std::runtime_error("Premature UTF-8 sequence end");
    if ((*mCurrent & 0xC0) != 0x80)
      throw std::runtime_error("Invalid UTF-8 continuation character");
    return *(mCurrent++) & 0x3F;
  }

  const std::uint8_t *mPrevCurrent = nullptr;
  const std::uint8_t *mCurrent = nullptr;
  const std::uint8_t *mEnd = nullptr;
  rune_t mNext;
};

} // namespace Cougar::Utils
