#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message);
};

class SaveLoadException : public GameException {
public:
    explicit SaveLoadException(const std::string& message);
};

#endif