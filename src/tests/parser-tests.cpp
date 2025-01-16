#include <catch2/catch_test_macros.hpp>

#include "../slisp/parser.hpp"

using namespace Slisp::Lexer;
using namespace Slisp::Types;
using namespace Slisp::Parser;

TEST_CASE("Empty program") {
    std::string input = "";
    Cons expected { };

    Lexer lxr { input };

    Cons* result = parse(lxr);
    CHECK(*result == static_cast<const Cons&>(expected));
}
