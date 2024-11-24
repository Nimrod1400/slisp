#include <iostream>

#include "lexer.hpp"
// #include "vm.hpp"
#include "types.hpp"
#include <unordered_map>

using namespace Slisp;

int main(int argc, char **argv) {
    (void) argc;
    (void) argv;

    using Env = std::unordered_map<std::string, Types::Procedure*>;

    std::function<Types::Value*(std::vector<Types::Value*>&)>
        plus_func = [](std::vector<Types::Value*> &nums) {
            Types::Number *res = new Types::Number { };
            for (const auto num : nums) {
                *res = *res + *(dynamic_cast<Types::Number*>(num));
            }
            return dynamic_cast<Types::Value*>(res);
        };

    Types::Procedure plus_proc = { plus_func };

    Env e = {
        { "+", &plus_proc },
    };

    Types::Number num1 = { 1 };
    Types::Number num2 = { 2 };
    Types::Number num3 = { 3 };
    std::vector<Types::Value*> args = { &num1, &num2, &num3 };

    Types::Procedure *f = (*e.find("+")).second;
    Types::Value *result =
        (*f)(args);

    std::cout << result->to_string() << std::endl;
}
