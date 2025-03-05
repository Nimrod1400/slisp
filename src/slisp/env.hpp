#ifndef ENV_HPP_
#define ENV_HPP_

#include <unordered_map>
#include <string>

#include "types.hpp"

namespace Slisp::Env {
    Types::Value* symbol_lookup(const Types::Symbol* symbol);

    void insert_value(const Types::Symbol* key, const Types::Value* val);

    void init();
} // namespace Slisp::Env

#endif // ENV_HPP_
