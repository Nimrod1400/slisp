#include <algorithm>
#include <stdexcept>
#include <variant>
#include "exceptions.hpp"
#include "lexer.hpp"

namespace Slisp::Lexer {
    Lexeme::Lexeme() :
        value { "" },
        lexeme_type { LexemeType::Empty }
    { }

    Lexeme::Lexeme(std::string_view value) :
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
        default:
            lexeme_type = LexemeType::Atom;
            break;
        }
    }

    Lexer::Lexer(const std::string &input) :
        m_input { input },
        m_it { m_input.cbegin() },
        m_no_previous_lexeme { true }
    { }

    Lexeme Lexer::m_lexicalize_paren() {
        std::size_t len = 1;
        Lexeme out {
            std::string_view { m_it, m_it + len },
        };
        m_it += len;
        return out;
    }

    Lexeme Lexer::m_lexicalize_comment() {
        auto comment_end = std::find(m_it + 1, m_input.end(), '\n');
        Lexeme out {
            std::string_view { m_it, comment_end },
        };
        m_it += std::distance(m_it, comment_end);
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
        };

        std::size_t len = std::distance(m_it, atom_end);
        m_it += len;
        return out;
    }

    Lexeme Lexer::peek_lexeme() {
        if (m_no_previous_lexeme) {
            return read_lexeme();
        }
        else {
            return m_previous_lexeme;
        }
    }

    Lexeme Lexer::read_lexeme() {
        for (;;) {
            if (m_it == m_input.cend()) {
                throw Exceptions::Eof { "Uncaught end of file" };
            }
            if (*m_it == ' ' || *m_it == '\t' || *m_it == '\n') {
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
            m_previous_lexeme = out;
            m_no_previous_lexeme = false;
            return out;
        }
        case ';': {
            Lexeme out = m_lexicalize_comment();
            m_previous_lexeme = out;
            m_no_previous_lexeme = false;
            return out;
        }
        default: {
            Lexeme out = m_lexicalize_atom();
            m_previous_lexeme = out;
            m_no_previous_lexeme = false;
            return out;
        }
        }
    }
}
