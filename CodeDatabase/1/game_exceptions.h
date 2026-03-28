#pragma once
#include <stdexcept>
#include <string>

// Базовое исключение для игровых ошибок
class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message) 
        : std::runtime_error("Game error: " + message) {}
};

// Исключения для файловых операций
class GameFileException : public GameException {
public:
    explicit GameFileException(const std::string& message) 
        : GameException("File operation failed: " + message) {}
};

class GameSaveException : public GameFileException {
public:
    explicit GameSaveException(const std::string& message) 
        : GameFileException("Save failed: " + message) {}
};

class GameLoadException : public GameFileException {
public:
    explicit GameLoadException(const std::string& message) 
        : GameFileException("Load failed: " + message) {}
};

// Исключения для данных игры
class GameDataException : public GameException {
public:
    explicit GameDataException(const std::string& message) 
        : GameException("Invalid game data: " + message) {}
};

class GameStateException : public GameException {
public:
    explicit GameStateException(const std::string& message) 
        : GameException("Invalid game state: " + message) {}
};