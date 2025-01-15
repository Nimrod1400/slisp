#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <string>
#include <cstdint>
#include <functional>

namespace Slisp::Types {
    enum class TypeOfValue {
        Symbol,
        Procedure,
        Cons,
    };

    class Value {
    public:
        virtual TypeOfValue get_type() const = 0;
        virtual std::string to_string() const = 0;

        virtual bool operator==(const Value& other) const = 0;

        virtual ~Value() { }

        bool reachable;
    };

    class Symbol : public Value {
    public:
        Symbol(const std::string& value);
        Symbol(const std::string_view value);

        bool operator==(const Value& other) const override;

        TypeOfValue get_type() const override;
        std::string to_string() const override;

    private:
        std::string m_str;
    };

    class Cons : public Value {
    public:
        Cons(Value* car = nullptr, Value* cdr = nullptr);

        bool operator==(const Value& other) const override;

        TypeOfValue get_type() const override;
        std::string to_string() const override;

        void set_car(Value *val);
        void set_cdr(Value *val);

        Value* car() const;
        Value* cdr() const;

    private:
        Value *m_car;
        Value *m_cdr;
    };

    class Procedure : public Value {
    public:
        Procedure();
        Procedure(const std::function<Value* (Cons*)>& func);

        bool operator==(const Value& other) const override;

        TypeOfValue get_type() const override;
        std::string to_string() const override;

    private:
        std::function<Value*(Cons*)> m_func;
    };
}

#endif // TYPES_HPP_
