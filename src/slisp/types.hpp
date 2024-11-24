#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <cstdarg>
#include <functional>

namespace Slisp::Types {
    class Value {
    public:
        virtual std::string to_string() const = 0;
        virtual ~Value() { }
    };

    class Datum : public Value {
    };

    class Number : public Datum {
    public:
        Number() {
            m_value = 0;
        }
        Number(int n) {
            m_value = n;
        }

        Number operator+(const Number &n) { return m_value + n.m_value; }
        Number operator-(const Number &n) { return m_value - n.m_value; }
        Number operator*(const Number &n) { return m_value * n.m_value; }
        Number operator/(const Number &n) { return m_value / n.m_value; }

        std::string to_string() const override {
            return std::to_string(m_value);
        }

    private:
        int m_value;
    };

    class Procedure : public Datum {
    public:
        Procedure(std::function<Value*(std::vector<Value*>&)> &proc) {
            m_proc = proc;
        }

        Value* operator()(std::vector<Value*>& args) {
            return m_proc(args);
        }

        std::string to_string() const {
            return "<Procedure>";
        }

    private:
        std::function<Value*(std::vector<Value*>&)> m_proc;
    };

    class Pair : public Value {
    public:
        void set_car(const Value *val);
        void set_cdr(const Value *val);

        Pair cons(const Value *val);

        Value* car() const;
        Value* cdr() const;

        std::string to_string() const override {
            return "(" + m_head->to_string() +
                " . " + m_tail->to_string();
        }

    private:
        Value *m_head;
        Value *m_tail;
    };
}

#endif // TYPES_HPP_
