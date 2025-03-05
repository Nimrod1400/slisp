#ifndef EVAL_HPP_
#define EVAL_HPP_

#include "types.hpp"

namespace Slisp::Eval {
    Types::Value* eval(Types::Value* expr);
} // namespace Slisp::Eval

#endif // EVAL_HPP_
