#ifndef SAVELOADEXCEPTION_H
#define SAVELOADEXCEPTION_H

#include <stdexcept>
#include <string>

class SaveLoadException : public std::runtime_error {
public:
    explicit SaveLoadException(const std::string &msg) : std::runtime_error(msg) {}
};

#endif // SAVELOADEXCEPTION_H
