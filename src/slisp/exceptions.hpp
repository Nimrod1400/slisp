#ifndef EXCEPTIONS_HPP_
#define EXCEPTIONS_HPP_

#include <stdexcept>
#include <utility>
#include <cstdint>

namespace Slisp::Exceptions {
    std::string form_error_message(const std::string &msg,
                                   std::size_t col,
                                   std::size_t row);

    class Exception : public std::exception {
    public:
        Exception(std::string message);

        const char* what() const noexcept override;

    private:
        std::string m_message;

    };
    
    class Eof : public Exception {
    public:
        Eof(const std::string &message);
    };

    class UnmatchedQuote : public Exception {
    public:
        UnmatchedQuote(const std::string &message);
    };
}

#endif // EXCEPTIONS_HPP_
