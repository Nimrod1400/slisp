#include "types.hpp"

namespace Slisp::Types {
    Symbol::Symbol(const std::string& value)
        : m_str { value }
    { }

    Symbol::Symbol(const std::string_view value)
        : m_str { value }
    { }

    bool Symbol::operator==(const Symbol& other) const {
        return m_str == other.m_str;
    }

    TypeOfValue Symbol::get_type() const {
        return TypeOfValue::Symbol;
    }

    std::string Symbol::to_string() const {
        return m_str;
    }

    Cons::Cons(Value* car, Value* cdr) :
        m_car { car },
        m_cdr { cdr }
    { }

    bool Cons::operator==(const Cons& other) const {
        return m_car == other.m_car &&
            m_cdr == other.m_cdr;
    }

    TypeOfValue Cons::get_type() const {
        return TypeOfValue::Cons;
    }

    void Cons::set_car(Value* val) { m_car = val; }

    void Cons::set_cdr(Value* val) { m_cdr = val; }

    Value* Cons::car() const { return m_car; }

    Value* Cons::cdr() const { return m_cdr; }

    std::string Cons::to_string() const {
        std::string out = "(";

        if (m_car == nullptr) {
            out += "";
        }
        else {
            out += m_car->to_string();
        }

        if (m_cdr == nullptr) {
            out += "'()";
        }
        else {
            out += m_cdr->to_string();
        }

        return out;
    }

    Procedure::Procedure()
        : Procedure([](Cons* args) { return args; })
    { }

    Procedure::Procedure(const std::function<Value* (Cons*)>& func)
        : m_func { func }
    { }

    bool Procedure::operator==(const Procedure&) const {
        return false;
    }

    TypeOfValue Procedure::get_type() const {
        return TypeOfValue::Procedure;
    }

    std::string Procedure::to_string() const {
        return "<Procedure>";
    }
}
