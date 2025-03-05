#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <string>
#include <cstdint>
#include <functional>

#include "lexer.hpp"

namespace Slisp::Types {
    enum class TypeOfValue {
        Symbol,
        Number,
        Procedure,
        Cons,
    };

    class Value {
    public:
        virtual TypeOfValue get_type() const = 0;
        virtual std::string to_string() const = 0;

        virtual bool equalp(const Value* other) const = 0;

        virtual ~Value() { }

        bool reachable;
    };

    class Symbol : public Value {
    public:
        Symbol(const std::string& value);
        Symbol(const std::string_view value);

        bool equalp(const Value* other) const override;

        TypeOfValue get_type() const override { return TypeOfValue::Symbol; }
        std::string to_string() const override;

    private:
        std::string m_str;
    };

    class Number : public Value {
    public:
        Number(int n);
        Number(const std::string& str);

        bool equalp(const Value* other) const override;

        TypeOfValue get_type() const override { return TypeOfValue::Number; }
        std::string to_string() const override;

        Number* operator+(const Number* rhs);
        Number* operator-(const Number* rhs);
        Number* operator*(const Number* rhs);
        Number* operator/(const Number* rhs);

        static bool numberp(const Lexer::Lexeme& lm);

    private:
        int m_number;
    };

    class Cons : public Value {
    public:
        Cons(Value* car = nullptr, Value* cdr = nullptr);

        bool equalp(const Value* other) const override;

        TypeOfValue get_type() const override { return TypeOfValue::Cons; }
        std::string to_string() const override;

        void set_car(Value* val);
        void set_cdr(Value* val);

        Value* car() const;
        Value* cdr() const;

    private:
        Value* m_car;
        Value* m_cdr;
    };

    class Procedure : public Value {
    public:
        Procedure();
        Procedure(const std::function<Value* (Cons*)>& func);

        bool equalp(const Value* other) const override;

        TypeOfValue get_type() const override { return TypeOfValue::Procedure; }
        std::string to_string() const override;

        Value* apply(Cons* args);

    private:
        std::function<Value*(Cons*)> m_func;
    };
}

#endif // TYPES_HPP_
