#include "parser.hpp"
#include "vm.hpp"

namespace Slisp::Parser {
    Types::Number* parse_number(Lexer::Lexer& lxr) {
        auto vm = VirtualMachine::VirtualMachine::instance();
        std::string s { lxr.read_lexeme().value };
        return vm.push<Types::Number>(s);
    }

    Types::Value* parse_atom(Lexer::Lexer& lxr) {
        return parse_number(lxr);
    }

    Types::Cons* parse_cons(Lexer::Lexer& lxr) {
        auto vm = VirtualMachine::VirtualMachine::instance();
        Types::Cons* out = vm.push<Types::Cons>();
        Types::Cons* last_cons = out;

        while (lxr.read_lexeme().lexeme_type != Lexer::LexemeType::RParen) {
            last_cons->set_car(parse(lxr));
            Types::Cons* new_cons = vm.push<Types::Cons>();
            last_cons->set_cdr(new_cons);
            last_cons = new_cons;
        }

        return out;
    }

    Types::Value* parse(Lexer::Lexer& lxr) {
        const Lexer::Lexeme lm = lxr.peek_lexeme();

        if (lm.lexeme_type == Lexer::LexemeType::LParen) {
            return parse_cons(lxr);
        }
        else {
            return parse_atom(lxr);
        }
    }
}
