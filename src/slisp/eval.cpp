#include "eval.hpp"
#include "env.hpp"

namespace Slisp::Eval {
    using namespace Slisp::Types;
    using namespace Slisp::Env;

    static Value* apply_proc(Value* expr) {
        Cons* e = static_cast<Cons*>(expr);
        Symbol* proc_name = static_cast<Symbol*>(e->car());
        Procedure* proc = static_cast<Procedure*>(symbol_lookup(proc_name));

        e = static_cast<Cons*>(e->cdr());
        Cons* args = new Cons();
        Cons* last_cons = args;

        for (; e != nullptr; e = static_cast<Cons*>(e->cdr())) {
            Cons* new_cons = new Cons();
            last_cons->set_car(eval(e->car()));
            last_cons->set_cdr(new_cons);
            last_cons = new_cons;
        }

        return proc->apply(args);
    }

    Value* eval(Value* expr) {
        TypeOfValue type = expr->get_type();

        if (type == TypeOfValue::Cons) {
            return apply_proc(expr);
        }
        else if (type == TypeOfValue::Cons) {
            return expr;
        }
        else if (type == TypeOfValue::Number) {
            return expr;
        }
        else if (type == TypeOfValue::Procedure) {
            return expr;
        }
    }
} // namespace Slisp::Eval
