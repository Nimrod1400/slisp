#include "exceptions.hpp"
#include "lexer.hpp"

namespace Slisp::Lexer {
    LexemeValue::LexemeValue(const std::string_view sv) :
        m_owns { false },
        m_value { sv }
    { }

    LexemeValue::LexemeValue(const std::string str) :
        m_owns { true },
        m_value { str }
    { }

    bool LexemeValue::operator==(const std::string &rhs) const {
        if (m_owns) {
            return std::get<std::string>(m_value) == rhs;
        }
        else {
            return std::get<std::string_view>(m_value) == rhs;
        }
    }

    std::ostream &operator<<(std::ostream &os, const LexemeValue &lv) {
        os << lv.c_str();
        return os;
    }

    const char *LexemeValue::c_str() const {
        if (m_owns) {
            return std::get<std::string>(m_value).c_str();
        }
        else {
            return std::get<std::string_view>(m_value).data();
        }
    }

    Lexer::Lexer(const std::string &input) :
        m_input { input },
        m_it { m_input.cbegin() },
        m_prev_lexeme { read_lexeme() },
        m_row { 1 },
        m_col { 1 }
    {
        reset_position();
    }

    Lexeme::Lexeme(std::size_t row, std::size_t col, LexemeValue value) :
        row { row },
        col { col },
        value { value } { }

    Lexeme Lexer::m_lexicalize_paren() {
        std::size_t len = 1;

        Lexeme out {
            m_row,
            m_col,
            std::string_view { m_it, m_it + len }
        };

        m_col += len;
        m_it++;

        return out;
    }

    Lexeme Lexer::m_lexicalize_comment() {
        std::size_t len = 1;

        while (m_it + len != m_input.cend() &&
               *(m_it + len) != '\n') {
            len += 1;
        }

        Lexeme out {
            m_row,
            m_col,
            std::string_view { m_it, m_it + len }
        };

        m_it += len;
        m_row += 1;
        m_col = 1;

        return out;
    }

    
    std::optional<std::string>
    Lexer::m_get_escaped(const std::string::const_iterator &it,
                         std::string::const_iterator &prev_it)
    {
        std::string esc_char;

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
            return std::nullopt;
        }

        std::string result { prev_it, it - 1 };
        result += esc_char;
        prev_it = it + 1;

        return result;
    }

    LexemeValue
    Lexer::m_escape_chars_in_str(std::string::const_iterator str_begin,
                                 std::string::const_iterator str_end)
    {
        if (*(str_end - 1) == '\\') {
            throw Slisp::Exceptions::UnmatchedQuote {
                Exceptions::form_error_message("Unmatched quote", m_row, m_col),
            };
        }

        bool ec_present = false;
        std::string escaped_str;
        auto it = str_begin + 1;
        auto prev_it = str_begin;

        for (; it != str_end; it++) {
            if (*it != '\\') {
                continue;
            }

            it++;
            std::optional<std::string> escaped_opt = m_get_escaped(it, prev_it);
            if (escaped_opt) {
                ec_present = true;
                escaped_str += *escaped_opt;
            }
        }

        if (ec_present) {
            escaped_str += std::string { prev_it, str_end };
            return escaped_str;
        }
        else {
            return std::string_view { str_begin, str_end };
        }
    }

    Lexeme Lexer::m_lexicalize_string_literal() {
        std::size_t len = 1;

        while (m_it + len != m_input.cend() &&
               *(m_it + len) != '\n' &&
               *(m_it + len) != '"') {
            len += 1;
        }

        if (m_it + len == m_input.cend() || *(m_it + len) == '\n') {
            throw Slisp::Exceptions::UnmatchedQuote {
                Exceptions::form_error_message("Unmatched quote", m_row, m_col),
            };
        }

        Lexeme out {
            m_row,
            m_col,
            m_escape_chars_in_str(m_it, m_it + len + 1),
        };

        m_col += len + 1;
        m_it += len + 1;

        return out;
    }

    Lexeme Lexer::m_lexicalize_atom() {
        std::size_t len = 1;

        while (m_it + len != m_input.cend() &&
               *(m_it + len) != ' '  &&
               *(m_it + len) != '\n' &&
               *(m_it + len) != '\t' &&
               *(m_it + len) != ')'  &&
               *(m_it + len) != '('  &&
               *(m_it + len) != ';'  &&
               *(m_it + len) != '"') {
            len += 1;
        }

        Lexeme out = {
            m_row,
            m_col,
            std::string_view { m_it, m_it + len },
        };

        m_it += len;
        m_col += len;

        return out;
    }

    void Lexer::reset_position() {
        m_it = m_input.cbegin();
    }

    Lexeme Lexer::peek_lexeme() const {
        return m_prev_lexeme;
    }

    Lexeme Lexer::read_lexeme() {
        while (m_it != m_input.cend() &&
               (*m_it == ' ' ||
                *m_it == '\t' ||
                *m_it == '\n')) {
            m_it++;
        }

        if (m_it == m_input.cend()) {
            throw Exceptions::Eof { "Uncaught end of file" };
        }

        switch (*m_it) {
        case '(':
        case ')':
            return m_lexicalize_paren();
        case ';':
            return m_lexicalize_comment();
        case '"':
            return m_lexicalize_string_literal();
        default:
            return m_lexicalize_atom();
        }
    }
}