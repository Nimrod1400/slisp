#include <list>
#include "parser.hpp"

namespace Slisp::Parser {
    using namespace Slisp::Types;
    using namespace Slisp::Lexer;

    Cons* parse_cons(Lexer::Lexer& lxr) {
        Cons* out = new Cons();
        Cons* last_cons = out;

        Lexeme lm = lxr.read_lexeme();

        if (lm.lexeme_type != LexemeType::RParen &&
            lm.lexeme_type != LexemeType::Empty) {
            out->set_car(parse(lxr));
            lm = lxr.read_lexeme();
        }

        while (lm.lexeme_type != LexemeType::RParen &&
               lm.lexeme_type != LexemeType::Empty) {
            Cons* new_cons = new Cons(parse(lxr));
            last_cons->set_cdr(new_cons);
            last_cons = new_cons;

            lm = lxr.read_lexeme();
        }

        return out;
    }

    Symbol* parse_symbol(Lexer::Lexer& lxr) {
        Symbol* result;
        Lexeme lm = lxr.peek_lexeme();

        result = new Symbol(lm.value);

        return result;
    }

    Value* parse(Lexer::Lexer& lxr) {
        Lexeme lm = lxr.peek_lexeme();

        if (lm.lexeme_type == LexemeType::LParen) {
            return parse_cons(lxr);
        }
        else if (lm.lexeme_type == LexemeType::Empty) {
            return new Cons();
        }
        else {
            return parse_symbol(lxr);
        }
    }
}
