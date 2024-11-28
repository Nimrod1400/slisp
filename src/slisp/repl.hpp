#ifndef REPL_HPP_
#define REPL_HPP_

#include <iostream>
#include "types.hpp"

namespace Slisp::Repl {

    std::string read();

    Types::Value* eval(const std::string& s);

    void print(Types::Value* v);

    void repl();

    void repl_loop();
}

#endif // REPL_HPP_
