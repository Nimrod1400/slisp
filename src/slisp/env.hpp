#ifndef ENV_HPP_
#define ENV_HPP_

#include <unordered_map>

namespace Slisp::Env {
    using Env = std::unordered_map<std::string, Types::Value*>;
    void populate_env(Env& env);
}

#endif // ENV_HPP_
