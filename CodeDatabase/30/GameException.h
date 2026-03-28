#pragma once
#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message) 
        : std::runtime_error(message) {}
};

class GameSaveException : public GameException {
public:
    explicit GameSaveException(const std::string& message) 
        : GameException(message) {}
};

class QuitGameException : public std::exception {
public:
    const char* what() const noexcept override { return "Game Quit"; }
};

