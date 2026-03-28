#ifndef SAVE_EXCEPTIONS_H
#define SAVE_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class SaveException : public std::runtime_error {
public:
    explicit SaveException(const std::string& msg) : std::runtime_error("SaveException: " + msg) {}
};

class LoadException : public std::runtime_error {
public:
    explicit LoadException(const std::string& msg) : std::runtime_error("LoadException: " + msg) {}
};

class FileIOException : public std::runtime_error {
public:
    explicit FileIOException(const std::string& msg) : std::runtime_error("FileIOException: " + msg) {}
};

#endif // SAVE_EXCEPTIONS_H
