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
            std::vector<Number*> nums;

            for (; args->cdr() != nullptr; args = static_cast<Cons*>(args->cdr())) {
                nums.push_back(static_cast<Number*>(args->car()));
            }

            Number* result = new Number(0);

            for (auto num : nums) {
                *result = *result + *num;
            }

            return result;
        };

        insert_value("+", new Procedure(plus_func));
    }

} // namespace Slisp::Env
