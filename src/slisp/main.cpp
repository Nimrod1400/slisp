#include <iostream>

#include "lexer.hpp"
#include <unordered_map>

using namespace Slisp;

int main(int argc, char** argv) {
    std::string input = "(1 2 (3 4 (5 6) (7 8 9)))";

    Lexer::Lexer lxr { input };

    while (lxr.read_lexeme().lexeme_type != Lexer::LexemeType::Empty) {
        std::cout << lxr.peek_lexeme().value << "\n";
    }
}
