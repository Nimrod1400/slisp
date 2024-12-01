#include <iostream>

#include "parser.hpp"
#include "lexer.hpp"
#include "vm.hpp"
#include "types.hpp"
#include "env.hpp"
#include <unordered_map>

using namespace Slisp;

auto vm = VirtualMachine::VirtualMachine::instance();

int main(int argc, char **argv) {
    std::string input = "(1 2 (3 4 (5 6) (7 8 9)))";

    Lexer::Lexer lxr { input };
    Types::Value* val = Parser::parse(lxr);
    std::cout << val->to_string() << "\n";
}
