#include "env.hpp"

#include <stdexcept>

namespace Slisp::Env {
    using namespace Slisp::Types;

    Env::Env(Env* parent_env) :
        m_parent { parent_env }
    { }

    void Env::define(const Symbol* key, Value* val) {
        define(key->to_string(), val);
    }

    void Env::define(const std::string& key, Value* val) {
        m_kvs[key] = val;
    }

    Value* Env::lookup(const Symbol* symbol) {
        return lookup(symbol->to_string());
    }

    Value* Env::lookup(const std::string& symbol) {
        try {
            return m_kvs[symbol];
        }
        catch (const std::out_of_range& e) {
            if (m_parent) {
                return m_parent->lookup(symbol);
            }
            else {
                return new Symbol(std::string_view {"not-found"});
            }
        }
    }

} // namespace Slisp::Env
