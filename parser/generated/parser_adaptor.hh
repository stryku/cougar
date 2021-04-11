#pragma once

#include "Parser_generated.h"

#include "../lexer_adaptor.hh"

#include <fmt/core.h>

namespace Cougar_generated
{


inline void Parser_generated::error()
{
    fmt::print("Error\n");
}

inline int Parser_generated::lex()
{
    int t = ::Cougar::Parser::LexerAdaptor::lex();
    d_val_ = ::Cougar::Parser::LexerAdaptor::currentText();
    return t;
}

inline void Parser_generated::print()         
{
}

inline void Parser_generated::exceptionHandler(std::exception const &)         
{
    throw;             
}

}
