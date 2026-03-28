#ifndef LOADEXCEPTION_H
#define LOADEXCEPTION_H

#include <exception>
#include <string>

class LoadException : public std::exception {
public:
    LoadException(const std::string& message) : message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }

private:
    std::string message_;
};

#endif // LOADEXCEPTION_H
