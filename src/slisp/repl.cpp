#include "repl.hpp"
#include "env.hpp"
#include "parser.hpp"

namespace Slisp::Repl {
    using namespace Slisp::Types;
    using namespace Slisp::Lexer;
    using namespace Slisp::Parser;

    Value* read(Lexer::Lexer& lxr) {
        return parse(lxr);
    }

    Value* eval(Value* stmt) {
        Cons* s = static_cast<Cons*>(stmt);

        Procedure* proc = static_cast<Procedure*>(Env::symbol_lookup(static_cast<Symbol*>(s->car())));
        Cons* args = static_cast<Cons*>(s->cdr());

        return proc->apply(args);
    }

    std::string print(Value* val) {
        return val->to_string();
    }

} // namespace Slisp::Repl
