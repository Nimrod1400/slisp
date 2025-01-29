#include "repl.hpp"

namespace Slisp::Repl {
    using namespace Slisp::Types;
    using namespace Slisp::Lexer;
    using namespace Slisp::Parser;

    Cons* read(Lexer::Lexer& lxr) {
        return parse(lxr);
    }

    Cons* eval(Cons* stmt) {
        
    }

    std::string print(Cons* val) {
        return val.to_string();
    }

} // namespace Slisp::Repl
