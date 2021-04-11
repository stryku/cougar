#pragma once

#include "Parser.h"

#include "../lexer_adaptor.hh"

#include <fmt/core.h>

namespace Cougar_generated
{


inline void Parser::error()
{
    fmt::print("Error\n");
}

inline int Parser::lex()
{
    return ::Cougar::Parser::LexerAdaptor::lex();
}

inline void Parser::print()         
{
}

inline void Parser::exceptionHandler(std::exception const &)         
{
    throw;             
}

}
