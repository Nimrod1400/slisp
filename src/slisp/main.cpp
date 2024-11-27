#include <iostream>

#include "lexer.hpp"
#include "vm.hpp"
#include "types.hpp"
#include <unordered_map>

using namespace Slisp;

VirtualMachine::VirtualMachine vm;

Types::Value* plus_func(std::vector<Types::Value*>& args) {
    Types::Number* result = vm.push<Types::Number>();
    for (auto const n : args) {
        *result = *result + *static_cast<Types::Number*>(n);
    }
    return result;
}

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    std::function<Types::Value*(std::vector<Types::Value*>&)>
        plus_function = plus_func;

    auto* n1 = vm.track<Types::Number>(34);
    auto* n2 = vm.track<Types::Number>(35);
    auto* cons = vm.push<Types::Cons>(n1, n2);
    auto* plus_proc = vm.push<Types::Procedure>(plus_function);
    std::vector<Types::Value*> args { n1, n2 };
    Types::Number sum_number =
        *static_cast<Types::Number*>((*plus_proc)(args));
    cons->set_cdr(nullptr);
    cons->set_car(nullptr);
    auto* sum = vm.push<Types::Number>(Types::Number { sum_number });

    std::cout << sum->to_string() << std::endl;
}
