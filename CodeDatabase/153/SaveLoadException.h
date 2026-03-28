#ifndef SAVELOADEXCEPTION_H
#define SAVELOADEXCEPTION_H

#include <stdexcept>
#include <string>

class SaveLoadException : public std::runtime_error {
public:
    explicit SaveLoadException(const std::string& message)
        : std::runtime_error("Save/Load Error: " + message) {}
};

#endif 