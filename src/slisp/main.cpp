#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "types.hpp"
#include "env.hpp"
#include "repl.hpp"

#include <unordered_map>

using namespace Slisp;

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    Env::init();
    std::string input = "(+ 35 34)";
    Lexer::Lexer lxr { input };

    auto in = Repl::read(lxr);
    auto computed = Repl::eval(in);
    std::cout << computed->to_string();
}
