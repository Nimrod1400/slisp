#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <iostream>
#include <optional>
#include <iterator>
#include <string_view>
#include <variant>

namespace Slisp::Lexer {
    enum class LexemeType {
        Empty,
        LParen,
        RParen,
        Atom,
        Comment,
    };

    class Lexeme {
    public:
        Lexeme();
        Lexeme(std::string_view value);

        std::string_view value;
        LexemeType lexeme_type;
    };

    class Lexer {
    public:
        Lexer(const std::string& input);

        Lexeme peek_lexeme();
        Lexeme read_lexeme();

    private:
        const std::string& m_input;
        std::string::const_iterator m_it;

        bool m_has_previous_lexeme;
        Lexeme m_previous_lexeme;

        Lexeme m_lexicalize_paren();
        Lexeme m_lexicalize_comment();
        Lexeme m_lexicalize_atom();
    };
}

#endif // LEXER_HPP_
