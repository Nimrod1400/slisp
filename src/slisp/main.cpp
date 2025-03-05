#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "types.hpp"
#include "env.hpp"
#include "repl.hpp"
#include "eval.hpp"

#include <unordered_map>

using namespace Slisp;

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    Env::init();
    std::string input = "(+ 35 (+ 14 21))";
    Lexer::Lexer lxr { input };

    auto in = Repl::read(lxr);
    auto computed = Repl::eval(in);
    auto out = Repl::print(computed);

    std::cout << out;
}
