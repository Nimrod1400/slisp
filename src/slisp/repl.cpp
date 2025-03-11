#include "env.hpp"
#include "eval.hpp"
#include "parser.hpp"
#include "repl.hpp"

namespace Slisp::Repl {
    using namespace Slisp::Types;
    using namespace Slisp::Lexer;
    using namespace Slisp::Parser;
    using namespace Slisp::Env;

    Value* read(Lexer::Lexer& lxr) {
        return parse(lxr);
    }

    Value* eval(Value* expr, Env::Env* env) {
        return Eval::eval(expr, env);
    }

    std::string print(Value* val) {
        return val->to_string();
    }

} // namespace Slisp::Repl
