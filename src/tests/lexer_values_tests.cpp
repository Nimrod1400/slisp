#include <catch2/catch_test_macros.hpp>

#include "../slisp/exceptions.hpp"
#include "../slisp/lexer.hpp"

using namespace Slisp::Lexer;

using Output          = std::pair<std::string, LexemeType>;
using OutputVector    = std::vector<Output>;
using InputOutputPair = std::pair<std::string, OutputVector>;
using Bundle          = std::vector<InputOutputPair>;

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
              { "(", LexemeType::LParen },
          }
        },

        { "(((",
          {
              { "(", LexemeType::LParen },
              { "(", LexemeType::LParen },
              { "(", LexemeType::LParen },
          }
        },

        { ")))",
          {
              { ")", LexemeType::RParen },
              { ")", LexemeType::RParen },
              { ")", LexemeType::RParen },
          }
        },

        { "())()()(())",
          {
              { "(", LexemeType::LParen },
              { ")", LexemeType::RParen },
              { ")", LexemeType::RParen },
              { "(", LexemeType::LParen },
              { ")", LexemeType::RParen },
              { "(", LexemeType::LParen },
              { ")", LexemeType::RParen },
              { "(", LexemeType::LParen },
              { "(", LexemeType::LParen },
              { ")", LexemeType::RParen },
              { ")", LexemeType::RParen },
          }
        },
    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };
        for (const auto &[val, lt] : pair.second) {
            CHECK(lexer.read_lexeme().value == LexemeValue { val, lt });
        }

        CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::Eof);
    }
}

TEST_CASE("Lexing comments") {
    Bundle bundle = {
        { "; test",
          {
              { "; test", LexemeType::Comment },
          }
        },

        { ";;; test\n",
          {
              { ";;; test", LexemeType::Comment },
          }
        },

        { ";test \"test w/ quotes\"",
          {
              { ";test \"test w/ quotes\"", LexemeType::Comment },
          }
        },

        { "() ; test \n",
          {
              { "(",       LexemeType::LParen },
              { ")",       LexemeType::RParen },
              { "; test ", LexemeType::Comment },
          }
        },

        { "()\n; test\n()\n;test",
          {
              { "(",      LexemeType::LParen },
              { ")",      LexemeType::RParen },
              { "; test", LexemeType::Comment },
              { "(",      LexemeType::LParen },
              { ")",      LexemeType::RParen },
              { ";test",  LexemeType::Comment },
          }
        },
    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };
        for (const auto &[val, lt] : pair.second) {
            CHECK(lexer.read_lexeme().value == LexemeValue { val, lt });
        }

        CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::Eof);
    }
}

TEST_CASE("Lexing atoms") {
    Bundle bundle = {
        { "test",
          {
              { "test", LexemeType::Atom },
          }
        },

        { "test_1\ntest_2 test_3",
          {
              { "test_1", LexemeType::Atom },
              { "test_2", LexemeType::Atom },
              { "test_3", LexemeType::Atom },
          }
        },

        { "\n3.14159\n",
          {
              { "3.14159", LexemeType::Atom },
          }
        },

        { "(define (square x) (* x x))\n",
          {
              { "(",      LexemeType::LParen },
              { "define", LexemeType::Atom   },
              { "(",      LexemeType::LParen },
              { "square", LexemeType::Atom   },
              { "x",      LexemeType::Atom   },
              { ")",      LexemeType::RParen },
              { "(",      LexemeType::LParen },
              { "*",      LexemeType::Atom   },
              { "x",      LexemeType::Atom   },
              { "x",      LexemeType::Atom   },
              { ")",      LexemeType::RParen },
              { ")",      LexemeType::RParen },
          }
        },
    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };

        for (const auto &[val, lt] : pair.second) {
            CHECK(lexer.read_lexeme().value == LexemeValue { val, lt });
        }
        CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::Eof);
    }
}

TEST_CASE("Lexing strings") {
    Bundle bundle = {
        { R"("test")",
          {
              { R"("test")", LexemeType::StringLiteral },
          }
        },

        { R"("test" ())",
          {
              { R"("test")", LexemeType::StringLiteral },
              { "(",         LexemeType::LParen        },
              { ")",         LexemeType::RParen        },
          }
        },

        { R"("test \n test again" ( ))",
          {
              { "\"test \n test again\"", LexemeType::StringLiteral },
              { "(",                      LexemeType::LParen        },
              { ")",                      LexemeType::RParen        },
          }
        },

        { R"("test \\ test again" ( ))",
          {
              { R"("test \ test again")", LexemeType::StringLiteral },
              { "(", LexemeType::LParen                             },
              { ")", LexemeType::RParen                             },
          }
        },

        { R"("test \t test again" ( ))",
          {
              { "\"test \t test again\"", LexemeType::StringLiteral },
              { "(",                      LexemeType::LParen        },
              { ")",                      LexemeType::RParen        },
          }
        },

        { R"((define name "Vadim Shamray"))",
          {
              { "(",                  LexemeType::LParen        },
              { "define",             LexemeType::Atom          },
              { "name",               LexemeType::Atom          },
              { R"("Vadim Shamray")", LexemeType::StringLiteral },
              { ")",                  LexemeType::RParen        },
         }
        },

        { R"((define name"Vadim Shamray"))",
          {
              { "(",                  LexemeType::LParen        },
              { "define",             LexemeType::Atom          },
              { "name",               LexemeType::Atom          },
              { R"("Vadim Shamray")", LexemeType::StringLiteral },
              { ")",                  LexemeType::RParen        },
          }
        },
    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };

        for (const auto &[val, lt] : pair.second) {
            CHECK(lexer.read_lexeme().value == LexemeValue { val, lt });
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
            CHECK_THROWS_AS(lexer.read_lexeme(),
                            Slisp::Exceptions::UnmatchedQuote);
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
                { ";; This procedure reverses list.", LexemeType::Comment },
                { "(",                                LexemeType::LParen  },
                { "define",                           LexemeType::Atom    },
                { "(",                                LexemeType::LParen  },
                { "reverse-list",                     LexemeType::Atom    },
                { "lst",                              LexemeType::Atom    },
                { ")",                                LexemeType::RParen  },
                { "(",                                LexemeType::LParen  },
                { "define",                           LexemeType::Atom    },
                { "(",                                LexemeType::LParen  },
                { "iter",                             LexemeType::Atom    },
                { "rest",                             LexemeType::Atom    },
                { "result",                           LexemeType::Atom    },
                { ")",                                LexemeType::RParen  },
                { "(",                                LexemeType::LParen  },
                { "cond",                             LexemeType::Atom    },
                { "(",                                LexemeType::LParen  },
                { "(",                                LexemeType::LParen  },
                { "nul?",                             LexemeType::Atom    },
                { "rest",                             LexemeType::Atom    },
                { ")",                                LexemeType::RParen  },
                { "result",                           LexemeType::Atom    },
                { ")",                                LexemeType::RParen  },
                { "(",                                LexemeType::LParen  },
                { "else",                             LexemeType::Atom    },
                { "(",                                LexemeType::LParen  },
                { "iter",                             LexemeType::Atom    },
                { "(",                                LexemeType::LParen  },
                { "cdr",                              LexemeType::Atom    },
                { "rest",                             LexemeType::Atom    },
                { ")",                                LexemeType::RParen  },
                { "(",                                LexemeType::LParen  },
                { "cons",                             LexemeType::Atom    },
                { "(",                                LexemeType::LParen  },
                { "car",                              LexemeType::Atom    },
                { "rest",                             LexemeType::Atom    },
                { ")",                                LexemeType::RParen  },
                { "result",                           LexemeType::Atom    },
                { ")",                                LexemeType::RParen  },
                { ")",                                LexemeType::RParen  },
                { ")",                                LexemeType::RParen  },
                { "(",                                LexemeType::LParen  },
                { "iter",                             LexemeType::Atom    },
                { "lst",                              LexemeType::Atom    },
                { "'",                                LexemeType::Atom    },
                { "(",                                LexemeType::LParen  },
                { ")",                                LexemeType::RParen  },
                { ")",                                LexemeType::RParen  },
                { ")",                                LexemeType::RParen  },
                { ")",                                LexemeType::RParen  },
                { ")",                                LexemeType::RParen  },
            }
        }
    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };

        for (const auto &[val, lt] : pair.second) {
            CHECK(lexer.read_lexeme().value == LexemeValue { val, lt });
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
                { "(",                      LexemeType::LParen        },
                { "define",                 LexemeType::Atom          },
                { "(",                      LexemeType::LParen        },
                { "form-greeting-message",  LexemeType::Atom          },
                { "name",                   LexemeType::Atom          },
                { ")",                      LexemeType::RParen        },
                { "(",                      LexemeType::LParen        },
                { "string-append",          LexemeType::Atom          },
                { R"("Oh hello there, ")",  LexemeType::StringLiteral },
                { "name",                   LexemeType::Atom          },
                { ")",                      LexemeType::RParen        },
                { ")",                      LexemeType::RParen        },
            }
        }
    };

    for (const auto &pair : bundle) {
        Lexer lexer { pair.first };
        std::cout << pair.first << std::endl;

        for (const auto &[val, lt] : pair.second) {
            Lexeme l = lexer.read_lexeme();
            CHECK(lexer.read_lexeme().value == LexemeValue { val, lt });
        }
        CHECK_THROWS_AS(lexer.read_lexeme(), Slisp::Exceptions::Eof);
    }
}
