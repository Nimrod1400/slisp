#include <catch2/catch_test_macros.hpp>

#include <string>
#include "../slisp/lexer.hpp"

#include <stdexcept>
#include <unordered_map>

using namespace Slisp::Lexer;

typedef std::vector<std::pair<std::string, std::vector<std::string>>> Bundle;

TEST_CASE("Lexing empty program") {
    std::vector<std::string> inputs = {
        "",
        " ",
        "\n",
        "\t \n   ",
        "  \t \n\n\t     ",
        "     \n",
    };
    for (const auto &input : inputs) {
        REQUIRE_THROWS_AS(Lexer { input }, std::out_of_range);
    }
}

TEST_CASE("Lexing parenthesis values") {
    Bundle bundle = {
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
            REQUIRE(lexer.read_lexeme().value == output);
        }
    }
}
TEST_CASE("Lexing comments values") {
    Bundle bundle = {
        { "; test",
          {
              "; test",
          }
        },

        { "; test\n",
          {
              "; test",
          }
        },

        { "() ; test\n",
          {
              "(", ")", "; test",
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
            REQUIRE(lexer.read_lexeme().value == output);
        }
    }
}

TEST_CASE("Lexing atoms values") {
    Bundle bundle = {
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
    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };
        for (const auto &output : pair.second) {
            REQUIRE(lexer.read_lexeme().value == output);
        }
    }
}
