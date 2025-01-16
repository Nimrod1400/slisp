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

    Cons* parse(Lexer::Lexer& lxr) {
        Lexeme lm = lxr.peek_lexeme();
        Cons* out = new Cons();
        Cons* last_cons = out;

        if (lm.lexeme_type != LexemeType::Empty) {
            Value* val;

            if (lm.lexeme_type == LexemeType::LParen) {
                val = parse_cons(lxr);
            }
            else {
                val = parse_symbol(lxr);
            }

            last_cons->set_cdr(val);
        }

        while (lm.lexeme_type != LexemeType::Empty) {
            Value* val;

            if (lm.lexeme_type == LexemeType::LParen) {
                val = parse_cons(lxr);
            }
            else {
                val = parse_symbol(lxr);
            }

            Cons* new_cons = new Cons(val);
            last_cons->set_cdr(new_cons);
            last_cons = new_cons;

            lm = lxr.peek_lexeme();
        }

        return out;
    }
}
