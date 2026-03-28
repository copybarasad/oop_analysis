#ifndef GAMEEXCEPTIONS_H
#define GAMEEXCEPTIONS_H

#include <stdexcept>
#include <string>

class GameFileException : public std::runtime_error {
public:
    explicit GameFileException(const std::string& message) 
        : std::runtime_error("File error: " + message) {}
};

class GameLoadException : public GameFileException {
public:
    explicit GameLoadException(const std::string& message) 
        : GameFileException("Load failed - " + message) {}
};

class GameSaveException : public GameFileException {
public:
    explicit GameSaveException(const std::string& message) 
        : GameFileException("Save failed - " + message) {}
};

class GameCorruptedException : public GameLoadException {
public:
    explicit GameCorruptedException(const std::string& message) 
        : GameLoadException("Corrupted data - " + message) {}
};

#endif