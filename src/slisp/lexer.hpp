#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <string_view>
#include <iterator>

namespace Slisp::Lexer {
    class Lexeme {
    public:
        Lexeme(std::size_t row, std::size_t col, std::string_view value) :
            row { row },
            col { col },
            value { value } { }

        std::size_t row;
        std::size_t col;
        std::string_view value;
    };

    class Lexer {
    public:
        Lexer(const std::string &input);

        void reset_position();
        Lexeme peek_lexeme() const;
        Lexeme read_lexeme();

    private:
        std::size_t m_row;
        std::size_t m_col;

        const std::string &m_input;
        std::string::const_iterator m_it;
        Lexeme m_prev_lexeme;

        Lexeme m_lexicalize_paren();
        Lexeme m_lexicalize_comment();
        Lexeme m_lexicalize_atom();
    };
}

#endif //LEXER_HPP_
