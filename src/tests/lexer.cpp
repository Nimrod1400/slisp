#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../slisp/exceptions.hpp"
#include "../slisp/lexer.hpp"

using namespace Slisp::Lexer;

using ValueBundle = std::vector<std::pair<std::string, std::vector<std::string>>>;
using PositionBundle = std::vector<std::pair<std::string, std::vector<std::pair<std::size_t, std::size_t>>>>;

TEST_CASE("Lexing empty program") {
    std::vector<std::string> inputs = {
        "",
        " ",
        "\n",
        "\t \n   ",
        "  \t \n\n\t     ",
        "     \n",
        "\t\t\t",
    };

    for (const auto &input : inputs) {
        CHECK_THROWS_AS(Lexer { input }, Slisp::Exceptions::Eof);
    }
}

TEST_CASE("Lexing parenthesis") {
    ValueBundle bundle = {
        { "(",
          {
              "(",
          }
        },

        { "(((",
          {
              "(", "(", "(",
          }
        },

        { ")))",
          {
              ")", ")", ")",
          }
        },

        { "())()()(())",
          {
              "(", ")", ")", "(", ")", "(",
              ")", "(", "(", ")", ")",
          }
        },
    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };
        for (const auto &output : pair.second) {
            CHECK(lexer.read_lexeme().value == output);
        }

        CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::Eof);
    }
}

TEST_CASE("Lexing comments") {
    ValueBundle bundle = {
        { "; test",
          {
              "; test",
          }
        },

        { ";test \"test in quotes\"",
          {
              ";test \"test in quotes\"",
          }
        },

        { "; test\n",
          {
              "; test",
          }
        },

        { "() ; test \n",
          {
              "(", ")", "; test ",
          }
        },

        { "()\n; test\n()\n;test",
          {
              "(", ")", "; test", "(", ")", ";test",
          }
        },
    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };
        for (const auto &output : pair.second) {
            CHECK(lexer.read_lexeme().value == output);
        }
        CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::Eof);
    }
}

TEST_CASE("Lexing atoms") {
    ValueBundle bundle = {
        { "test",
          {
              "test",
          }
        },

        { "test_1\ntest_2 test_3",
          {
              "test_1", "test_2", "test_3",
          }
        },

        { "\n3.14159\n",
          {
              "3.14159",
          }
        },

        { "(define (square x) (* x x))\n",
          {
              "(", "define", "(", "square", "x", ")",
              "(", "*", "x", "x", ")", ")",
          }
        },

        { "(\n    atom;comment \n)",
          {
              "(", "atom", ";comment ", ")",
          }
        },

    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };

        for (const auto &output : pair.second) {
            CHECK(lexer.read_lexeme().value == output);
        }
        CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::Eof);
    }
}

TEST_CASE("Lexing erronous strings") {
    SECTION("First lexeme is erronoues string") {
        std::vector<std::string> inputs = {
            R"(")",
            R"( " )",
            R"(" \")"
        };

        for (const auto &input : inputs) {
            CHECK_THROWS_AS(Lexer { input }, Slisp::Exceptions::UnmatchedQuote);
        }
    }

    SECTION("Second lexeme is erronoues string") {
        std::vector<std::string> inputs = {
            R"(text ")",
            R"(text " )",
            R"(text " \" )",
        };

        for (const auto &input : inputs) {
            Lexer lexer { input };
            lexer.read_lexeme();
            CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::UnmatchedQuote);
        }
    }
}

TEST_CASE("Lexing strings") {
    ValueBundle bundle = {
        { R"("test")",
          {
              R"("test")",
          }
        },

        { R"("test" ())",
          {
              R"("test")", "(", ")",
          }
        },

        { R"("test \n test again" ( ))",
          {
              "\"test \n test again\"", "(", ")",
          }
        },

        { R"("test \\ test again" ( ))",
          {
              R"("test \ test again")", "(", ")",
          }
        },

        { R"("test \t test again" ( ))",
          {
              "\"test \t test again\"", "(", ")",
          }
        },

        { R"((define name "Vadim Shamray"))",
          {
              "(", "define", "name", R"("Vadim Shamray")", ")",
         }
        },

        { R"((define name"Vadim Shamray"))",
          {
              "(", "define", "name", R"("Vadim Shamray")", ")",
          }
        },
    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };
        for (const auto &output : pair.second) {
          CHECK(lexer.read_lexeme().value == output);
        }

        CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::Eof);
    }
}

TEST_CASE("Lexemes positions") {
    SECTION("Parenthesis positions") {
        PositionBundle bundle = {
            {
                "()()",
                {
                    { 1, 1 },
                    { 1, 2 },
                    { 1, 3 },
                    { 1, 4 },
                }
            },
        };

        for (const auto &pair : bundle) {
            Lexer lexer { pair.first };
            for (const auto [row, col] : pair.second) {
                CHECK(lexer.read_lexeme().row == row);
                CHECK(lexer.peek_lexeme().col == col);
            }
        }
    }
}
