#include <list>
#include <charconv>

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

    Value* parse_atom(Lexer::Lexer& lxr) {
        Value* out;
        Lexeme lm = lxr.peek_lexeme();

        int n;
        const char* first = lm.value.data();
        const char* last = lm.value.data() + lm.value.size();
        auto [_, ec] = std::from_chars(first, last, n);

        if (ec == std::errc()) {
            out = new Number(n);
        }
        else {
            out = new Symbol(lm.value);
        }

        return out;
    }

    Value* parse(Lexer::Lexer& lxr) {
        Lexeme lm = lxr.peek_lexeme();

        if (lm.lexeme_type == LexemeType::LParen) {
            return parse_cons(lxr);
        }
        else if (lm.lexeme_type == LexemeType::Empty) {
            return nullptr;
        }
        else {
            return parse_atom(lxr);
        }
    }
}
