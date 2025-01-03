#include "types.hpp"

namespace Slisp::Types {
    Number::Number() : m_value { 0 }
    { }

    Number::Number(long long n) : m_value { n }
    { }

    Number::Number(const std::string& s) :
        Number::Number { std::stoll(s) }
    { }

    Number::Number(std::string_view sv) :
        Number::Number { std::string { sv } }
    { }

    TypeOfValue Number::get_tag() const {
        return TypeOfValue::Number;
    }

    Number Number::operator+(const Number &n) {
        return Number { m_value + n.m_value };
    }
    Number Number::operator-(const Number &n) {
        return Number { m_value - n.m_value };
    }
    Number Number::operator*(const Number &n) {
        return Number { m_value * n.m_value };
    }
    Number Number::operator/(const Number &n) {
        return Number { m_value / n.m_value };
    }

    Procedure::Procedure(std::function<Value*(Value*, Value*...)>& func) :
    m_func{ func }
    { }

    TypeOfValue Procedure::get_tag() const {
        return TypeOfValue::Procedure;
    }

    Cons::Cons(Value* car, Value* cdr) :
        m_car { car },
        m_cdr { cdr }
    { }

    TypeOfValue Cons::get_tag() const {
        return TypeOfValue::Cons;
    }

    void Cons::set_car(Value* val) { m_car = val; }

    void Cons::set_cdr(Value* val) { m_cdr = val; }

    Value* Cons::car() const { return m_car; }

    Value* Cons::cdr() const { return m_cdr; }

    std::string Cons::to_string() const {
        std::string out =
            "(" +
            m_car->to_string() +
            " . " +
            m_cdr->to_string() +
            ")";

        return out;
    }
}
