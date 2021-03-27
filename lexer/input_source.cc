#include "input_source.hh"

#include <fmt/core.h>

#include <cstring>
#include <stdexcept>

namespace Cougar::Lexer {

InputSource::InputSource(const std::string &path) {
  mFile = std::fopen(path.c_str(), "r");

  if (!mFile) {
    throw std::runtime_error(fmt::format("Error opening input file '{}' : {}",
                                         path, std::strerror(errno)));
  }
}

InputSource::~InputSource() {
  if (mFile)
    std::fclose(mFile);
}

} // namespace Cougar::Lexer