#include <iostream>

#include "lexer.hpp"
#include "vm.hpp"
#include "types.hpp"
#include <unordered_map>

using namespace Slisp;

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    VirtualMachine::VirtualMachine vm;

    auto* n1 = vm.track<Types::Number>(34);
    auto* n2 = vm.track<Types::Number>(35);
    auto* cons = vm.push<Types::Cons>(n1, n2);

    std::cout << "Slispy Lisp!\n";
}
