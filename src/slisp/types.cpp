#include "types.hpp"

namespace Slisp::Types {
    Number::Number() : m_value { 0 }
    { }

    Number::Number(std::int_fast64_t n) : m_value { n }
    { }

    Number::Number(const std::string& s) : Number::Number { std::stoi(s) }
    { }

    Number::Number(const std::string_view& s) {
    }

    bool Number::is_reachable() const {
        return m_reachable;
    }

    void Number::mark_reachable() {
        m_reachable = true;
    }

    void Number::mark_unreachable() {
        m_reachable = false;
    }

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

    bool Procedure::is_reachable() const {
        return m_reachable;
    }

    void Procedure::mark_reachable() {
        m_reachable = true;
    }

    void Procedure::mark_unreachable() {
        m_reachable = false;
    }

    TypeOfValue Procedure::get_tag() const {
        return TypeOfValue::Procedure;
    }

    Cons::Cons() { }

    Cons::Cons(Value* car, Value* cdr) :
        m_car { car },
        m_cdr { cdr }
    { }

    bool Cons::is_reachable() const {
        return m_reachable;
    }

    void Cons::mark_reachable() {
        m_reachable = true;
        if(m_car) m_car->mark_reachable();
        if(m_cdr) m_cdr->mark_reachable();
    }

    void Cons::mark_unreachable() {
        m_reachable = false;
    }

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
