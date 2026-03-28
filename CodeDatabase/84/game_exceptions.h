#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message) : std::runtime_error(message) {}
};

class SaveLoadException : public GameException {
public:
    explicit SaveLoadException(const std::string& message) 
        : GameException("Ошибка сохранения/загрузки: " + message) {}
};

#endif