#ifndef REPL_HPP_
#define REPL_HPP_

namespace Slisp::Repl {
    Types::Cons* read(Lexer::Lexer& lxr);
    Types::Cons* eval(Types::Cons* stmt);
    std::string print(Types::Cons* val);
} // namespace Slisp::Repl

#endif // REPL_HPP_
