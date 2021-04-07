#pragma once

#include <string>
#include <vector>

namespace Cougar::Utils {
class ZoneAllocator;
}

namespace Cougar::Lexer {

class Token;

// Processes a input file, places token in memory zone,
// returns vector of pointers to tokens.
// Throws on error
std::vector<Token *> lexBuffer(std::string_view buffer,
                               Utils::ZoneAllocator &zone);

} // namespace Cougar::Lexer
