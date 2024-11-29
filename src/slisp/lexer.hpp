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
        StringLiteral,
        Comment,
    };

    class Lexeme {
    public:
        Lexeme();
        Lexeme(std::string_view value, std::size_t row, std::size_t col);

        std::size_t row;
        std::size_t col;
        std::string_view value;
        LexemeType lexeme_type;
    };

    class Lexer {
    public:
        Lexer(const std::string &input);

        void reset_position();
        Lexeme peek_lexeme();
        Lexeme read_lexeme();

    private:
        std::size_t m_row;
        std::size_t m_col;

        const std::string &m_input;
        std::string::const_iterator m_it;

        bool m_no_prev_lexeme;
        Lexeme m_prev_lexeme;

        Lexeme m_lexicalize_paren();
        Lexeme m_lexicalize_comment();
        Lexeme m_lexicalize_string_literal();
        Lexeme m_lexicalize_atom();

        char
        m_get_escaped(const std::string::const_iterator &it);

        std::string_view
        m_escape_chars_in_str(std::string::const_iterator str_begin,
                              std::string::const_iterator str_end);
    };
}

#endif // LEXER_HPP_
