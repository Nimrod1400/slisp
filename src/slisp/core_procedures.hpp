#ifndef CORE_PROCEDURES_HPP_
#define CORE_PROCEDURES_HPP_

#include "types.hpp"

namespace Slisp::CoreProcedures {
    using namespace Slisp::Types;
    Value* plus_proc(std::vector<Value*>& args);
    Value* minus_proc(std::vector<Value*>& args);
    Value* multiply_proc(std::vector<Value*>& args);
    Value* divide_proc(std::vector<Value*>& args);
}

#endif // CORE_PROCEDURES_HPP_
