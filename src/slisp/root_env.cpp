#include "root_env.hpp"

#include "types.hpp"

namespace Slisp::Env {
    using namespace Slisp::Types;

    Value* plus_def(Cons* args) {
        Number* result = args->car<Number>();
        args = args->cdr<Cons>();

        for (; args->car<Value>(); args = args->cdr<Cons>()) {
            *result = *result + *args->car<Number>();
        }

        return result;
    }

    Value* minus_def(Cons* args) {
        Number* result = args->car<Number>();
        args = args->cdr<Cons>();

        for (; args->car<Value>(); args = args->cdr<Cons>()) {
            *result = *result - *args->car<Number>();
        }

        return result;
    }

    Value* mult_def(Cons* args) {
        Number* result = args->car<Number>();
        args = args->cdr<Cons>();

        for (; args->car<Value>(); args = args->cdr<Cons>()) {
            *result = *result * *args->car<Number>();
        }

        return result;
    }

    Value* div_def(Cons* args) {
        Number* result = args->car<Number>();
        args = args->cdr<Cons>();

        for (; args->car<Value>(); args = args->cdr<Cons>()) {
            *result = *result / *args->car<Number>();
        }

        return result;
    }

    void populate_root_env(Env& root_env) {
        root_env.define("+", new Procedure(plus_def));
        root_env.define("-", new Procedure(minus_def));
        root_env.define("*", new Procedure(mult_def));
        root_env.define("/", new Procedure(div_def));
    }
} // namespace Slisp::Env
