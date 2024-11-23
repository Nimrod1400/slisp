#include <catch2/catch_test_macros.hpp>

#include "../slisp/exceptions.hpp"
#include "../slisp/lexer.hpp"

using namespace Slisp::Lexer;

using Position = std::pair<std::size_t, std::size_t>;
using InputOutputPair = std::pair<std::string, std::vector<Position>>;
using Bundle = std::vector<InputOutputPair>;

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
                Lexeme lm = lexer.read_lexeme();
                CHECK(lm.row == row);
                CHECK(lm.col == col);
            }
        }
    }

    SECTION("Almost real life data") {
        Bundle bundle = {
            {
                "(define (square x)\n"
                "\t(* x x))",
                {
                    { 1, 1 },
                    { 1, 2 },
                    { 1, 9 },
                    { 1, 10 },
                    { 1, 17 },
                    { 1, 18 },
                    { 2, 2 },
                    { 2, 3 },
                    { 2, 5 },
                    { 2, 7 },
                    { 2, 8 },
                    { 2, 9 },
                }
            },
        };

        for (const auto &pair : bundle) {
            Lexer lexer { pair.first };
            for (const auto &[row, col] : pair.second) {
                Lexeme lm = lexer.read_lexeme();
                CHECK(lm.row == row);
                CHECK(lm.col == col);
            }
        }
    }
}
