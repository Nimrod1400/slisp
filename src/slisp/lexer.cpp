#include "lexer.hpp"

static Lexeme lexicalize_paren(const std::string &input, std::string::iterator &pos) {
    pos++;
    return;
}

static Lexeme lexicalize_comment(const std::string &input, std::string::iterator &pos) {
    
}

static Lexeme lexicalize_atom(const std::string &input, std::string::iterator &pos) {
    
}

namespace Slisp::Lexer {
    Lexeme Lexer::peek_lexeme() const {
        
    }

    Lexeme Lexer::read_lexeme() {
        
    }
}
