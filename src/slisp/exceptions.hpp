#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

#include <stdexcept>
#include <utility>

namespace Slisp::Exceptions {
    class Exception : public std::exception {
    public:
        Exception(std::string message) : m_message { std::move(message) } { }

        const char* what() const noexcept override {
            return m_message.c_str();
        }

    private:
        std::string m_message;

    };
    
    class Eof : public Exception {
    public:
        Eof(const std::string &message) : Exception { message } { }
    };

    class UnmatchedQuotationMark : public Exception {
    public:
        UnmatchedQuotationMark(const std::string &message) : Exception { message } { }
    };
}

#endif // EXCEPTIONS_HPP_
