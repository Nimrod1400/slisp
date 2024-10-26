#include "exceptions.hpp"

namespace Slisp::Exceptions {
    std::string form_error_message(const std::string &msg,
                                   std::size_t col,
                                   std::size_t row) {
        return msg +
            std::string { " at " } +
            std::to_string(col)    +
            std::string { ":" }    +
            std::to_string(row);
    }

    Exception::Exception(std::string message) :
        m_message { std::move(message) } { }

    const char* Exception::what() const noexcept {
        return m_message.c_str();
    }

    Eof::Eof(const std::string &message) : Exception { message } { }

    UnmatchedQuote::UnmatchedQuote(const std::string &message) :
        Exception { message } { }
}
