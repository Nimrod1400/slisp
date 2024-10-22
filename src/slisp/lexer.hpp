#ifndef LEXER_HPP_
#define LEXER_HPP_

#include <string_view>
#include <iterator>

namespace Slisp::Lexer {
    class EOF { };
    
    class Lexeme {
    public:
        std::string value;
    };

    class Lexer {
    public:
        Lexer(const std::string &input) : m_input { input } { }

        Lexeme peek_lexeme() const;
        Lexeme read_lexeme();

    private:
        const std::string &m_input;
        std::string::iterator m_position;
    };
}

#endif //LEXER_HPP_
