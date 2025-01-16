#ifndef PARSER_HPP_
#define PARSER_HPP_

#include "types.hpp"
#include "lexer.hpp"

namespace Slisp::Parser {
    Types::Cons* parse(Lexer::Lexer& lxr);
}

#endif // PARSER_HPP_
