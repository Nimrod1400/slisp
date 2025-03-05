#include "env.hpp"
#include "eval.hpp"
#include "parser.hpp"
#include "repl.hpp"

namespace Slisp::Repl {
    using namespace Slisp::Types;
    using namespace Slisp::Lexer;
    using namespace Slisp::Parser;

    Value* read(Lexer::Lexer& lxr) {
        return parse(lxr);
    }

    Value* eval(Value* expr) {
        return Eval::eval(expr);
    }

    std::string print(Value* val) {
        return val->to_string();
    }

} // namespace Slisp::Repl
