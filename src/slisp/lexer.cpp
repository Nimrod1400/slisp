#include "lexer.hpp"

#include "exceptions.hpp"

namespace Slisp::Lexer {
    Lexer::Lexer(const std::string &input) : m_input { input },
                                             m_it { m_input.cbegin() },
                                             m_prev_lexeme { read_lexeme() },
                                             m_row { 1 },
                                             m_col { 1 } {
        reset_position();
    }

    Lexeme::Lexeme(std::size_t row, std::size_t col, std::string_view value) :
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

    Lexeme Lexer::m_lexicalize_string() {
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
            std::string_view { m_it, m_it + len + 1}
        };

        m_it += len + 1;
        m_col += len + 1;

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

    Lexeme Lexer::peek_lexeme() const {
        return m_prev_lexeme;
    }

    void Lexer::reset_position() {
        m_it = m_input.cbegin();
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
            return m_lexicalize_string();
        default:
            return m_lexicalize_atom();
        }
    }
} // namespace Slisp::Lexer
