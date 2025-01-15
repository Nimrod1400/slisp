#include "types.hpp"

namespace Slisp::Types {
    Procedure::Procedure(std::function<Value*(Cons*)>& func) :
    m_func { func }
    { }

    Symbol::Symbol(const std::string& value)
        : m_str { value }
    { }

    Symbol::Symbol(const std::string_view value)
        : m_str { value }
    { }

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

    TypeOfValue Cons::get_type() const {
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

    TypeOfValue Procedure::get_type() const {
        return TypeOfValue::Procedure;
    }

    std::string Procedure::to_string() const {
        return "<Procedure>";
    }
}
