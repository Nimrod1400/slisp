#include "parser.hpp"

namespace Slisp::Parser {
    using namespace Slisp::Types;
    using namespace Slisp::Lexer;

    Cons* parse_cons(Lexer::Lexer& lxr) {
        Cons* cons;
    }

    Symbol* parse_symbol(Lexer::Lexer& lxr) {
        Symbol* result;
        Lexeme lm = lxr.peek_lexeme();

        result = new Symbol(lm.value);

        return result;
    }

    Value* parse(Lexer::Lexer& lxr) {
        Lexeme lm = lxr.read_lexeme();
        if (lm.lexeme_type == LexemeType::LParen) {
            return parse_cons(lxr);
        }
        else {
            return parse_symbol(lxr);
        }
    }
}
