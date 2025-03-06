#include "eval.hpp"
#include "env.hpp"

namespace Slisp::Eval {
    using namespace Slisp::Types;
    using namespace Slisp::Env;

    static Value* apply_proc(Procedure* proc, Cons* args) {
        Cons* ev_args = new Cons();
        Cons* tail = ev_args;

        for (; args != nullptr; args = args->cdr<Cons>()) {
            Cons* new_tail = new Cons();
            tail->set_car(eval(args->car<Value>()));
            tail->set_cdr(new_tail);
            tail = new_tail;
        }

        return proc->apply(ev_args);
    }

    Value* eval(Value* expr) {
        Value* out;
        TypeOfValue type = expr->get_type();

        if (type == TypeOfValue::Cons) {
            Cons* expr_cons = static_cast<Cons*>(expr);
            Symbol* p_name = static_cast<Symbol*>(expr_cons->car<Value>());
            Procedure* p = static_cast<Procedure*>(symbol_lookup(p_name));
            Cons* args = expr_cons->cdr<Cons>();

            out = apply_proc(p, args);
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
