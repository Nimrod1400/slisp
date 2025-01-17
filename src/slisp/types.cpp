#include "types.hpp"

namespace Slisp::Types {
    Symbol::Symbol(const std::string& value)
        : m_str { value }
    { }

    Symbol::Symbol(const std::string_view value)
        : m_str { value }
    { }

    bool Symbol::equalp(const Value* other) const {
        if (!other) {
            return false;
        }

        if (get_type() != other->get_type()) {
            return false;
        }

        const Symbol *other_s = static_cast<const Symbol*>(other);

        return m_str == other_s->m_str;
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

    static bool values_equalp(const Value* a, const Value* b) {
        bool both_nil = !a && !b;

        if (both_nil) {
            return true;
        }
        if (!a || !b) {
            return false;
        }

        return a->equalp(b);
    }

    bool Cons::equalp(const Value* other) const {
        if (get_type() != other->get_type()) {
            return false;
        }

        const Cons *other_c = static_cast<const Cons*>(other);

        return values_equalp(m_car, other_c->m_car) &&
            values_equalp(m_cdr, other_c->m_cdr);
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
            out += "'()";
        }
        else {
            out += m_car->to_string();
        }

        out += " . ";

        if (m_cdr == nullptr) {
            out += "'()";
        }
        else {
            out += m_cdr->to_string();
        }

        out += ")";

        return out;
    }

    Procedure::Procedure()
        : Procedure([](Cons* args) { return args; })
    { }

    Procedure::Procedure(const std::function<Value* (Cons*)>& func)
        : m_func { func }
    { }

    bool Procedure::equalp(const Value*) const {
        return false;
    }

    TypeOfValue Procedure::get_type() const {
        return TypeOfValue::Procedure;
    }

    std::string Procedure::to_string() const {
        return "<Procedure>";
    }
}
