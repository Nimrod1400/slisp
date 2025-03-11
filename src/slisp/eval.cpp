#include "eval.hpp"
#include "env.hpp"

namespace Slisp::Eval {
    using namespace Slisp::Types;

    Value* eval(Value* expr, Env::Env* env) {
        Value* out;
        TypeOfValue type = expr->get_type();

        if (type == TypeOfValue::Cons) {
            Symbol* p_name = expr->as<Cons>()->car<Symbol>();
            Procedure* proc = env->lookup(p_name)->as<Procedure>();
            Cons* args = expr->as<Cons>()->cdr<Cons>();

            Cons* ev_args = new Cons();
            Cons* tail = ev_args;

            for (; args != nullptr; args = args->cdr<Cons>()) {
                Cons* new_tail = new Cons();
                tail->set_car(eval(args->car<Value>(), env));
                tail->set_cdr(new_tail);
                tail = new_tail;
            }

            out = proc->apply(ev_args);
        }
        else if (type == TypeOfValue::Symbol) {
            Symbol* symbol = expr->as<Symbol>();
            out = env->lookup(symbol);
        }
        else if (type == TypeOfValue::Cons) {
            out = expr;
        }
        else if (type == TypeOfValue::Number) {
            out = expr;
        }
        else if (type == TypeOfValue::Procedure) {
            out = expr;
        }

        return out;
    }
} // namespace Slisp::Eval
