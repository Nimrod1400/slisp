#include <iostream>

#include "lexer.hpp"
#include "parser.hpp"
#include "types.hpp"

#include <unordered_map>

using namespace Slisp;

int main(int argc, char** argv) {
    (void) argc;
    (void) argv;

    std::string input = "((1 2) 3 4)";

    Lexer::Lexer lxr { input };
    Types::Value* val = Parser::parse(lxr);

    std::cout << val->to_string() << "\n";
}
