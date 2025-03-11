#ifndef ENV_HPP_
#define ENV_HPP_

#include <unordered_map>
#include <string>

#include "types.hpp"

namespace Slisp::Env {
    class Env {
    public:
        Env(Env* parent_env = nullptr);

        void define(const Types::Symbol* key, Types::Value* val);
        void define(const std::string& key, Types::Value* val);

        Types::Value* lookup(const Types::Symbol* symbol);
        Types::Value* lookup(const std::string& symbol);

    private:
        Env* m_parent;
        std::unordered_map<std::string, Types::Value*> m_kvs;
    };
} // namespace Slisp::Env

#endif // ENV_HPP_
