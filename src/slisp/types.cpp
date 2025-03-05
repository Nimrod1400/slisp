#include "types.hpp"

namespace Slisp::Types {
    Symbol::Symbol(const std::string& value) :
        m_str { value }
    { }

    Symbol::Symbol(const std::string_view value) :
        m_str { value }
    { }

    bool Symbol::equalp(const Value* other) const {
        if (!other) {
            return false;
        }

        if (get_type() != other->get_type()) {
            return false;
        }

        const Symbol* other_s = static_cast<const Symbol*>(other);

        return m_str == other_s->m_str;
    }

    std::string Symbol::to_string() const {
        return m_str;
    }

    Number::Number(int n) :
        m_number { n }
    { }

    Number::Number(const std::string& str) :
        Number::Number { std::stoi(str) }
    { }

    bool Number::equalp(const Value* other) const {
        if (!other) {
            return false;
        }

        if (get_type() != other->get_type()) {
            return false;
        }

        const Number* other_s = static_cast<const Number*>(other);

        return m_number == other_s->m_number;
    }

    std::string Number::to_string() const {
        return std::to_string(m_number);
    }

    Number* Number::operator+(const Number* rhs) {
        return new Number(m_number + rhs->m_number);
    }

    Number* Number::operator-(const Number* rhs) {
        return new Number(m_number - rhs->m_number);
    }

    Number* Number::operator*(const Number* rhs) {
        return new Number(m_number * rhs->m_number);
    }

    Number* Number::operator/(const Number* rhs) {
        return new Number(m_number / rhs->m_number);
    }

    Cons::Cons(Value* car, Value* cdr) :
        m_car { car },
        m_cdr { cdr }
    { }

    static bool values_equalp(const Value* a, const Value* b) {
        if (!a && !b) {
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

        const Cons* other_c = static_cast<const Cons*>(other);

        return values_equalp(m_car, other_c->m_car) &&
            values_equalp(m_cdr, other_c->m_cdr);
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

    std::string Procedure::to_string() const {
        return "<Procedure>";
    }

    Value* Procedure::apply(Cons* args) {
        return m_func(args);
    }
}
