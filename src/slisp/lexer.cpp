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
        default: {
            Lexeme out = m_lexicalize_atom();
            m_prev_lexeme = out;
            m_no_prev_lexeme = false;
            return out;
        }
        }
    }
}
