#include <catch2/catch_test_macros.hpp>

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
        "\t\t\t",
    };

    for (const auto &input : inputs) {
        CHECK_THROWS_AS(Lexer { input }, Slisp::Exceptions::Eof);
    }
}

TEST_CASE("Lexing parenthesis") {
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
            CHECK(lexer.read_lexeme().value == output);
        }

        CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::Eof);
    }
}

TEST_CASE("Lexing comments") {
    Bundle bundle = {
        { "; test",
          {
              "; test",
          }
        },

        { ";;; test\n",
          {
              ";;; test",
          }
        },

        { ";test \"test w/ quotes\"",
          {
              ";test \"test w/ quotes\"",
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
            CHECK(lexer.read_lexeme().value == output);
        }
        CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::Eof);
    }
}

TEST_CASE("Lexing strings") {
    Bundle bundle = {
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

TEST_CASE("Lexing list reversing procedure") {
    Bundle bundle = {
        {
            ";; This procedure reverses list.\n"
            "(define (reverse-list lst)\n"
            " (define (iter rest result)\n"
            "  (cond ((nul? rest) result)\n"
            "        (else (iter (cdr rest) (cons (car rest) result)))\n"
            "   (iter lst '()))))\n",
            {
                ";; This procedure reverses list.",
                "(", "define", "(", "reverse-list", "lst", ")",
                "(", "define", "(", "iter", "rest", "result", ")",
                "(", "cond", "(", "(", "nul?", "rest", ")", "result", ")",
                "(", "else", "(", "iter", "(", "cdr", "rest", ")", "(", "cons", "(", "car", "rest", ")", "result", ")", ")", ")",
                "(", "iter", "lst", "'", "(", ")", ")", ")", ")", ")",  
            }
        }
    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };
        for (const auto &output : pair.second) {
          CHECK(lexer.read_lexeme().value == output);
        }

        CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::Eof);
    }
}

TEST_CASE("Lexing string appending") {
    Bundle bundle = {
        {
            "(define (form-greeting-message name) \n"
            R"((string-append "Oh hello there, ")""\n"
            "                 name))\n",
            {
                "(", "define", "(", "form-greeting-message", "name", ")",
                "(", "string-append", R"("Oh hello there, ")",
                "name", ")", ")",
            }

        }
    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };
        for (const auto &output : pair.second) {
          CHECK(lexer.read_lexeme().value == output);
        }

        CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::Eof);
    }
}
