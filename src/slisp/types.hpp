#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <string>
#include <cstdarg>
#include <cstdint>
#include <functional>

namespace Slisp::Types {
    enum class TypeOfValue {
        Number,
        Procedure,
        Symbol,
        Cons,
    };

    class Value {
    public:
        virtual TypeOfValue get_tag() const = 0;
        virtual std::string to_string() const = 0;

        virtual ~Value() { }
    };

    class Number : public Value {
    public:
        Number();
        Number(long long n);
        Number(const std::string& s);
        Number(std::string_view sv);

        TypeOfValue get_tag() const override;

        Number operator+(const Number &n);
        Number operator-(const Number &n);
        Number operator*(const Number &n);
        Number operator/(const Number &n);

        std::string to_string() const override {
            return std::to_string(m_value);
        }

    private:
        long long m_value;
        bool m_reachable;
    };

    class Procedure : public Value {
    public:
        Procedure(std::function<Value*(Value*, Value*...)>& func);

        TypeOfValue get_tag() const override;

        std::string to_string() const {
            return "<Procedure>";
        }

    private:
        std::function<Value*(Value*, Value*...)>& m_func;

        bool m_reachable;
    };

    class Cons : public Value {
    public:
        Cons(Value* car = nullptr, Value* cdr = nullptr);

        TypeOfValue get_tag() const override;

        void set_car(Value *val);
        void set_cdr(Value *val);

        Value* car() const;
        Value* cdr() const;

        std::string to_string() const override;

    private:
        Value *m_car;
        Value *m_cdr;

        bool m_reachable;
    };
}

#endif // TYPES_HPP_
