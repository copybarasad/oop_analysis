#ifndef FILE_IOE_EXCEPTION_H
#define FILE_IOE_EXCEPTION_H

#include <stdexcept>
#include <string>

class FileIOException : public std::runtime_error
{
public:
    explicit FileIOException(const std::string& msg) : std::runtime_error(msg) {}
};

#endif