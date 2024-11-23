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

    SourceCodeException::SourceCodeException(const std::string &message) :
        m_message { m_message }
    { }

    const char* SourceCodeException::what() const noexcept {
        return m_message.c_str();
    }

    Eof::Eof(const std::string &message) : SourceCodeException { message }
    { }

    UnmatchedQuote::UnmatchedQuote(const std::string &message) :
        SourceCodeException { message }
    { }
}
