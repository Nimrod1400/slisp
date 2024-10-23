#include "lexer.hpp"

#include <stdexcept>

namespace Slisp::Lexer {
    Lexer::Lexer(const std::string &input) : m_input { input },
                                             m_it { m_input.cbegin() },
                                             m_prev_lexeme { read_lexeme() },
                                             m_row { 0 },
                                             m_col { 0 } {
        reset_position();
    }

    Lexeme Lexer::m_lexicalize_paren() {
        std::size_t len = 1;

        Lexeme out {
            m_row,
            m_col,
            std::string_view { m_it, m_it + 1 }
        };

        m_col += len;
        m_it++;

        return out;
    }

    Lexeme Lexer::m_lexicalize_comment() {
        
    }

    Lexeme Lexer::m_lexicalize_atom() {
        
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
            throw std::out_of_range { "EOF reached" };
        }

        switch (*m_it) {
        case '(':
        case ')':
            return m_lexicalize_paren();
        case ';':
            return m_lexicalize_comment();
        default:
            return m_lexicalize_atom();
        }
    }
} // namespace Slisp::Lexer
