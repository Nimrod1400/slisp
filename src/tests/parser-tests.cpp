#include <catch2/catch_test_macros.hpp>

#include "../slisp/parser.hpp"

using namespace Slisp::Lexer;
using namespace Slisp::Types;
using namespace Slisp::Parser;

template <typename T>
static Cons* strs_to_list(const T& lst) {
    Cons* out = new Cons();
    Cons* last_cons = out;

    if (lst.begin() == lst.end()) {
        return out;
    }

    Symbol* first_symbol = new Symbol(*lst.begin());
    out->set_car(first_symbol);

    for (auto it = lst.begin() + 1; it != lst.end(); ++it) {
        Symbol* new_value = new Symbol(*it);
        Cons* new_cons = new Cons(new_value);
        last_cons->set_cdr(new_cons);
        last_cons = new_cons;
    }

    return out;
}

template <typename T>
static Cons* vals_to_list(const T& lst) {
    Cons* out = new Cons();
    Cons* last_cons = out;

    if (lst.begin() == lst.end()) {
        return out;
    }

    out->set_car(*lst.begin());

    for (auto it = lst.begin() + 1; it != lst.end(); ++it) {
        Cons* new_cons = new Cons(*it);
        last_cons->set_cdr(new_cons);
        last_cons = new_cons;
    }

    return out;
}

TEST_CASE("Empty program") {
    std::string input = "";
    Cons expected { };

    Lexer lxr { input };

    const Cons* result = dynamic_cast<const Cons*>(parse(lxr));
    CHECK(result->equalp(&expected));
}

TEST_CASE("Single symbol") {
    std::string input = "hello-world";
    Symbol expected { input };

    Lexer lxr { input };

    const Symbol* result = dynamic_cast<const Symbol*>(parse(lxr));
    CHECK(result->equalp(&expected));
}

TEST_CASE("Pair") {
    std::string input = "(1 2)";
    std::vector<std::string> expected_values { "1", "2", };

    Cons* expected = strs_to_list<std::vector<std::string>>(expected_values);
    Lexer lxr { input };

    const Cons* result = dynamic_cast<const Cons*>(parse(lxr));
    CHECK(result->equalp(expected));
}

TEST_CASE("List") {
    std::string input = "(1 2 3 4 5)";
    std::vector<std::string> expected_values { "1", "2", "3", "4", "5", };

    Cons* expected = strs_to_list<std::vector<std::string>>(expected_values);
    Lexer lxr { input };

    const Cons* result = dynamic_cast<const Cons*>(parse(lxr));
    CHECK(result->equalp(expected));
}

TEST_CASE("List 2") {
    std::string input = "(1 2 3 4)";
    std::vector<std::string> expected_values { "1", "2", "3", "4", };

    Cons* expected = strs_to_list<std::vector<std::string>>(expected_values);
    Lexer lxr { input };

    const Cons* result = dynamic_cast<const Cons*>(parse(lxr));
    CHECK(result->equalp(expected));
}

TEST_CASE("List 3") {
    std::string input = " ( define name 'vadim ) ";
    std::vector<std::string> expected_values { "define", "name", "'vadim", };

    Cons* expected = strs_to_list<std::vector<std::string>>(expected_values);
    Lexer lxr { input };

    const Cons* result = dynamic_cast<const Cons*>(parse(lxr));
    CHECK(result->equalp(expected));
}

TEST_CASE("Tree") {
    std::string input = "(define (square x) (* x x))";

    std::vector<std::string> sublist_1 { "square", "x" };
    std::vector<std::string> sublist_2 { "*", "x", "x" };

    std::vector<Value*> expected_values = {
        new Symbol(std::string { "define"} ),
        strs_to_list(sublist_1),
        strs_to_list(sublist_2),
    };

    Cons* expected = vals_to_list(expected_values);

    Lexer lxr { input };

    const Cons* result = dynamic_cast<const Cons*>(parse(lxr));
    CHECK(result->equalp(expected));
}
