#include "core_procedures.hpp"

namespace Slisp::CoreProcedures {
    using namespace Slisp::Types;
    Value* plus_proc(std::vector<Value*>& args) {
        Types::Number* result = vm.push<Types::Number>();
        for (auto const n : args) {
            *result = *result + *static_cast<Types::Number*>(n);
        }
        return result;
    }

    Value* minus_proc(std::vector<Value*>& args) {
        Types::Number* result = vm.push<Types::Number>();
        for (auto const n : args) {
            *result = *result - *static_cast<Types::Number*>(n);
        }
        return result;
    }

    Value* multiply_proc(std::vector<Value*>& args) {
        Types::Number* result = vm.push<Types::Number>();
        for (auto const n : args) {
            *result = *result * *static_cast<Types::Number*>(n);
        }
        return result;
    }

    Value* divide_proc(std::vector<Value*>& args) {
        Types::Number* result = vm.push<Types::Number>();
        for (auto const n : args) {
            *result = *result / *static_cast<Types::Number*>(n);
        }
        return result;
    }
}

