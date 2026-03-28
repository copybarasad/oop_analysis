#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class GameFileException : public std::runtime_error {
public:
    explicit GameFileException(const std::string& message) 
        : std::runtime_error("File Error: " + message) {}
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

class InvalidSaveDataException : public GameLoadException {
public:
    explicit InvalidSaveDataException(const std::string& details) 
        : GameLoadException("Invalid save data: " + details) {}
};

#endif