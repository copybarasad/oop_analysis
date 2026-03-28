#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

class SaveException : public std::runtime_error {
public:
    SaveException(const std::string& msg) : std::runtime_error(msg) {}
};

class LoadException : public std::runtime_error {
public:
    LoadException(const std::string& msg) : std::runtime_error(msg) {}
};

#endif // EXCEPTIONS_H