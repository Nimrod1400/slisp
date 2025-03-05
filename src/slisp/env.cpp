#include "env.hpp"

#include <stdexcept>

namespace Slisp::Env {
    using namespace Slisp::Types;

    using Env = std::unordered_map<std::string, Value*>;

    Env env;

    Value* symbol_lookup(const Symbol* key) {
        try {
            return env[key->to_string()];
        }
        catch (const std::out_of_range& e) {
            return new Symbol(std::string_view {"not-found"});
        }
    }

    void insert_value(const Symbol* key, Value* val) {
        env[key->to_string()] = val;
    }

    void insert_value(const std::string& key, Value* val) {
        env[key] = val;
    }

    void init() {
        using Func = std::function<Value*(Cons*)>;

        Func plus_func = [](Cons* args) {
            int x = std::stoi(args->car()->to_string());
            int y = std::stoi(static_cast<Cons*>(args->cdr())->car()->to_string());

            Symbol* out = new Symbol(std::to_string(x + y));
            return out;
        };

        insert_value("+", new Procedure(plus_func));
    }

} // namespace Slisp::Env
