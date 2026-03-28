#ifndef LOAD_EXCEPTION_H
#define LOAD_EXCEPTION_H

#include <stdexcept>
#include <string>

class LoadException : public std::runtime_error
{
public:
    explicit LoadException(const std::string& msg) : std::runtime_error(msg) {}
};

#endif