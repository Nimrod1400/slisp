#include <catch2/catch_test_macros.hpp>

#include <string>
#include "../slisp/lexer.hpp"

using namespace Slisp::Lexer;

TEST_CASE("Lexer is lexing") {
    std::string input_1 = "()";
    Lexer lexer_1 { input_1 };

    REQUIRE(lexer_1.read_lexeme().value == "(");
    REQUIRE(lexer_1.read_lexeme().value == ")");
}
