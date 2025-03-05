#ifndef REPL_HPP_
#define REPL_HPP_

#include <string>

#include "types.hpp"
#include "lexer.hpp"

namespace Slisp::Repl {
    Types::Value* read(Lexer::Lexer& lxr);
    Types::Value* eval(Types::Value* stmt);
    std::string print(Types::Value* val);
} // namespace Slisp::Repl

#endif // REPL_HPP_
