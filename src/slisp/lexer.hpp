#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <iostream>
#include <optional>
#include <iterator>
#include <string_view>
#include <variant>

namespace Slisp::Lexer {
    class LexemeValue {
    public:
        LexemeValue(const std::string_view sv);
        LexemeValue(const std::string str);

        bool operator==(const std::string &rhs) const;

        friend std::ostream
        &operator<<(std::ostream &os, const LexemeValue &lv);

        bool owns() const;
        const std::variant<std::string, std::string_view> &access() const;
        const char *c_str() const;
    private:
        bool m_owns;
        std::variant<std::string, std::string_view> m_value;
    };
    
    class Lexeme {
    public:
        Lexeme(std::size_t row, std::size_t col, LexemeValue value);

        std::size_t row;
        std::size_t col;
        LexemeValue value;
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
        Lexeme m_lexicalize_string_literal();
        Lexeme m_lexicalize_atom();

        std::optional<std::string>
        m_get_escaped(const std::string::const_iterator &it,
                      std::string::const_iterator &prev_it);

        LexemeValue
        m_escape_chars_in_str(std::string::const_iterator str_begin,
                              std::string::const_iterator str_end);
    };
}

#endif //LEXER_HPP_
