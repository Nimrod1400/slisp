#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <string>
#include <cstdarg>
#include <functional>

namespace Slisp::Types {
    enum class ValueType {
        Number,
        Procedure,
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
        ValueType m_tag;
        bool m_reachable;
    };

    using SlispFunction = std::function<Value*(const std::vector<Value*>&)>;

    class Procedure : public Value {
    public:
        Procedure(SlispFunction &proc) {
            m_proc = proc;
        }

        bool is_reachable() const override;
        void mark_reachable() override;
        void mark_unreachable() override;

        ValueType get_tag() const override;

        Value* operator()(const std::vector<Value*>& args) {
            return m_proc(args);
        }

        std::string to_string() const {
            return "<Procedure>";
        }

    private:
        SlispFunction m_proc;

        ValueType m_tag;
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

        virtual void set_car(Value *val);
        virtual void set_cdr(Value *val);

        virtual Value* car() const;
        virtual Value* cdr() const;

        std::string to_string() const override;

    private:
        Value *m_car;
        Value *m_cdr;

        ValueType m_tag;
        bool m_reachable;
    };
}

#endif // TYPES_HPP_
