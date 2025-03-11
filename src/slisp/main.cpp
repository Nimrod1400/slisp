#include <iostream>

#include "env.hpp"
#include "eval.hpp"
#include "lexer.hpp"
#include "parser.hpp"
#include "repl.hpp"
#include "root_env.hpp"
#include "types.hpp"

#include <unordered_map>

using namespace Slisp;

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    std::string input = "(+ (* 1 1) (/ 12 6) (- 9 6))";
    Lexer::Lexer lxr { input };

    Env::Env root_env;
    Env::populate_root_env(root_env);

    auto in = Repl::read(lxr);
    auto computed = Repl::eval(in, &root_env);
    auto out = Repl::print(computed);

    std::cout << out;
}
