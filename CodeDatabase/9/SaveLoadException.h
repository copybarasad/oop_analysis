#ifndef SAVE_LOAD_EXCEPTION_H
#define SAVE_LOAD_EXCEPTION_H

#include <stdexcept>
#include <string>

class SaveLoadException : public std::runtime_error {
public:
    explicit SaveLoadException(const std::string& message): std::runtime_error(message) {}
};

#endif