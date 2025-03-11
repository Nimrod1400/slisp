#ifndef EVAL_HPP_
#define EVAL_HPP_

#include "types.hpp"
#include "env.hpp"

namespace Slisp::Eval {
    Types::Value* eval(Types::Value* expr, Env::Env* env);
} // namespace Slisp::Eval

#endif // EVAL_HPP_
