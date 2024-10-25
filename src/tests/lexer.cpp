#include <catch2/catch_test_macros.hpp>

#include <stdexcept>
#include <string>
#include <unordered_map>

#include "../slisp/exceptions.hpp"
#include "../slisp/lexer.hpp"

using namespace Slisp::Lexer;

using Bundle = std::vector<std::pair<std::string, std::vector<std::string>>>;

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
        REQUIRE_THROWS_AS(Lexer { input }, Slisp::Exceptions::Eof);
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

        { "(\n    atom;comment \n)",
          {
              "(", "atom", ";comment ", ")",
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

TEST_CASE("Lexing strings values") {
    Bundle bundle = {
        { "\"test\"",
          {
              "\"test\"",
          }
        },

        { "\"test\\n\"\n()",
          {
              "\"test\n\"", "(", ")",
          }
        },

        { "\"\\ttest\\n\"\n()",
          {
              "\"\ttest\n\"", "(", ")",
          }
        },

        { "(define name \"Vadim Shamray\")",
          {
              "(", "define", "name", "\"Vadim Shamray\"", ")",
         }
        },

        { "(define name\"Vadim Shamray\")",
          {
              "(", "define", "name", "\"Vadim Shamray\"", ")",
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
