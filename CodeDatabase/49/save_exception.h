#ifndef SAVE_EXCEPTION_H
#define SAVE_EXCEPTION_H

#include <stdexcept>
#include <string>

class SaveException : public std::runtime_error
{
public:
    explicit SaveException(const std::string& msg) : std::runtime_error(msg) {}
};

#endif