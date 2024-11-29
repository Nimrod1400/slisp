#include <algorithm>
#include <stdexcept>
#include <variant>
#include "exceptions.hpp"
#include "lexer.hpp"

namespace Slisp::Lexer {
    Lexeme::Lexeme() :
        row { 1 },
        col { 1 },
        value { "" },
        lexeme_type { LexemeType::Empty }
    { }

    Lexeme::Lexeme(std::string_view value, std::size_t row, std::size_t col) :
        row { row },
        col { col },
        value { value }
    {
        switch (value[0]) {
        case '(':
            lexeme_type = LexemeType::LParen;
            break;
        case ')':
            lexeme_type = LexemeType::RParen;
            break;
        case '"':
            lexeme_type = LexemeType::StringLiteral;
            break;
        case ';':
            lexeme_type = LexemeType::Comment;
            break;
        defaulexeme_type:
            lexeme_type = LexemeType::Atom;
            break;
        }
    }

    Lexer::Lexer(const std::string &input) :
        m_row { 1 },
        m_col { 1 },
        m_input { input },
        m_it { m_input.cbegin() },
        m_no_prev_lexeme { true }
    { }

    Lexeme Lexer::m_lexicalize_paren() {
        Lexeme out {
            std::string_view { m_it, m_it + 1},
            m_row,
            m_col,
        };

        std::size_t len = 1;

        m_col += len;
        m_it += len;

        return out;
    }

    Lexeme Lexer::m_lexicalize_comment() {
        auto comment_end = std::find(m_it + 1, m_input.end(), '\n');

        Lexeme out {
            std::string_view { m_it, comment_end },
            m_row,
            m_col,
        };

        m_it += std::distance(m_it, comment_end);
        m_row += 1;
        m_col = 1;

        return out;
    }

    char Lexer::m_get_escaped(const std::string::const_iterator &it)
    {
        char esc_char;

        switch (*it) {
        case '\\':
            esc_char = '\\';
            break;
        case '"':
            esc_char = '"';
            break;
        case 'n':
            esc_char = '\n';
            break;
        case 'r':
            esc_char = '\r';
            break;
        case 't':
            esc_char = '\t';
            break;
        case 'b':
            esc_char = '\b';
            break;
        case 'v':
            esc_char = '\v';
            break;
        case 'f':
            esc_char = '\f';
            break;
        default:
            esc_char = '\0';
        }

        return esc_char;
    }

    std::string_view
    Lexer::m_escape_chars_in_str(std::string::const_iterator str_begin,
                                 std::string::const_iterator str_end)
    {
        bool contains_esc_ch = false;
        auto prev_it = str_begin;
        std::string* escaped_str = new std::string { };

        for (auto it = str_begin + 1; it != str_end - 1; it++) {
            if (*it != '\\') {
                continue;
            }
            it++;

            char escaped_char = m_get_escaped(it);
            if (!escaped_char) {
                continue;
            }

            contains_esc_ch = true;
            escaped_str->append(std::string { prev_it, it - 1 });
            escaped_str->push_back(escaped_char);
            prev_it = it + 1;
        }

        if (contains_esc_ch) {
            escaped_str->append(std::string { prev_it, str_end });
            return std::string_view { escaped_str->begin(),
                                      escaped_str->end() };
        }
        else {
            delete escaped_str;
            return std::string_view { str_begin, str_end };
        }
    }

    Lexeme Lexer::m_lexicalize_string_literal() {
        constexpr auto is_str_end = [] (char c) {
            return c == '\n' || c == '"';
        };
        auto str_end = std::find_if(m_it + 1, m_input.cend(), is_str_end);

        if (*str_end != '"' || *(str_end - 1) == '\\') {
            throw Slisp::Exceptions::UnmatchedQuote {
                Exceptions::form_error_message("Unmatched quote", m_row, m_col),
            };
        }

        Lexeme out {
            m_escape_chars_in_str(m_it, str_end + 1),
            m_row,
            m_col,
        };

        std::size_t len = std::distance(m_it, str_end) + 1;
        m_col += len;
        m_it += len;

        return out;
    }

    Lexeme Lexer::m_lexicalize_atom() {
        constexpr auto is_atom_end = [] (char c) {
            return c == ' ' ||
                c == '\n' ||
                c == '\t' ||
                c == ')'  ||
                c == '('  ||
                c == ';'  ||
                c == '"';
        };
        auto atom_end = std::find_if(m_it + 1, m_input.cend(), is_atom_end);

        Lexeme out = {
            std::string_view { m_it, atom_end },
            m_row,
            m_col,
        };

        std::size_t len = std::distance(m_it, atom_end);

        m_it += len;
        m_col += len;

        return out;
    }

    void Lexer::reset_position() {
        m_it = m_input.cbegin();
        m_row = 1;
        m_col = 1;
    }

    Lexeme Lexer::peek_lexeme() {
        if (m_no_prev_lexeme) {
            return read_lexeme();
        }
        else {
            return m_prev_lexeme;
        }
    }

    Lexeme Lexer::read_lexeme() {
        for (;;) {
            if (m_it == m_input.cend()) {
                throw Exceptions::Eof { "Uncaught end of file" };
            }

            if (*m_it == ' ' || *m_it == '\t') {
                m_col += 1;
                m_it += 1;
            }
            else if (*m_it == '\n') {
                m_row += 1;
                m_col = 1;
                m_it += 1;
            }
            else {
                break;
            }
        }

        switch (*m_it) {
        case '(':
        case ')': {
            Lexeme out = m_lexicalize_paren();
            m_prev_lexeme = out;
            m_no_prev_lexeme = false;
            return out;
        }
        case ';': {
            Lexeme out = m_lexicalize_comment();
            m_prev_lexeme = out;
            m_no_prev_lexeme = false;
            return out;
        }
        case '"': {
            Lexeme out = m_lexicalize_string_literal();
            m_prev_lexeme = out;
            m_no_prev_lexeme = false;
            return out;
        }
        default: {
            Lexeme out = m_lexicalize_atom();
            m_prev_lexeme = out;
            m_no_prev_lexeme = false;
            return out;
        }
        }
    }
}
