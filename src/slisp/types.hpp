#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <string>
#include <cstdarg>
#include <functional>

namespace Slisp::Types {
    enum class ValueType {
        Number,
        Procedure,
        Symbol,
        String,
        Cons,
        Bits,
    };

    class Value {
    public:
        virtual bool is_reachable() const = 0;
        virtual void mark_reachable() = 0;
        virtual void mark_unreachable() = 0;

        virtual ValueType get_tag() const = 0;
        virtual std::string to_string() const = 0;

        virtual ~Value() { }
    };

    class Number : public Value {
    public:
        Number();
        Number(int n);
        Number(const std::string& s);
        Number(const std::string_view& s);

        bool is_reachable() const override;
        void mark_reachable() override;
        void mark_unreachable() override;

        ValueType get_tag() const override;

        Number operator+(const Number &n);
        Number operator-(const Number &n);
        Number operator*(const Number &n);
        Number operator/(const Number &n);

        std::string to_string() const override {
            return std::to_string(m_value);
        }

    private:
        int m_value;
        bool m_reachable;
    };

    class Procedure : public Value {
    public:
        Procedure(std::function<Value*(Value*, Value*...)>& func);
        bool is_reachable() const override;
        void mark_reachable() override;
        void mark_unreachable() override;

        ValueType get_tag() const override;

        Value* apply(Value* first, Value*... args);
        Value* apply();

        std::string to_string() const {
            return "<Procedure>";
        }

    private:
        std::function<Value*(Value*, Value*...)>& m_func;

        bool m_reachable;
    };

    class Cons : public Value {
    public:
        Cons();
        Cons(Value* car, Value* cdr = nullptr);

        bool is_reachable() const override;
        void mark_reachable() override;
        void mark_unreachable() override;

        ValueType get_tag() const override;

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
