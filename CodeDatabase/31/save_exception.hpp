#ifndef SAVE_EXCEPTION_HPP
#define SAVE_EXCEPTION_HPP

#include <exception>
#include <string>

class SaveException : public std::exception {
protected:
    std::string message;
public:
    explicit SaveException(const std::string& msg) : message(msg) {}
    const char* what() const noexcept override { return message.c_str(); }
};

#endif
