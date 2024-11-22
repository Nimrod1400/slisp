#include <catch2/catch_test_macros.hpp>

#include "../slisp/exceptions.hpp"
#include "../slisp/lexer.hpp"

using namespace Slisp::Lexer;

using Bundle = std::vector<std::pair<std::string, std::vector<std::pair<std::size_t, std::size_t>>>>;

TEST_CASE("Lexemes positions") {
    SECTION("Parenthesis positions") {
        Bundle bundle = {
            {
                "()()",
                {
                    { 1, 1 },
                    { 1, 2 },
                    { 1, 3 },
                    { 1, 4 },
                }
            },
            {
                "()\n()",
                {
                    { 1, 1 },
                    { 1, 2 },
                    { 2, 1 },
                    { 2, 2 },
                }
            }
        };

        for (const auto &pair : bundle) {
            Lexer lexer { pair.first };
            for (const auto &[row, col] : pair.second) {
                CHECK(lexer.read_lexeme().row == row);
                CHECK(lexer.peek_lexeme().col == col);
            }
        }
    }

    SECTION("Almost real life data") {
        Bundle bundle = {
            {
                "(define (square x)\n"
                "\t(* x x))",
                {
                    { 1, 1 }, { 1, 2 }, { 1, 9 }, { 1, 10 }, { 1, 17 }, { 1, 18 },
                    { 2, 2 }, { 2, 3 }, { 2, 5 }, { 2, 7 }, { 2, 8 }, { 2, 9 },
                }
            },
        };

        for (const auto &pair : bundle) {
            Lexer lexer { pair.first };
            for (const auto &[row, col] : pair.second) {
                CHECK(lexer.read_lexeme().row == row);
                CHECK(lexer.peek_lexeme().col == col);
            }
        }
    }
}
