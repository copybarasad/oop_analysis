#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message) : std::runtime_error(message) {}
};

class SaveException : public GameException {
public:
    explicit SaveException(const std::string& message) : GameException("Save error: " + message) {}
};

class LoadException : public GameException {
public:
    explicit LoadException(const std::string& message) : GameException("Load error: " + message) {}
};

class FileNotFoundException : public LoadException {
public:
    explicit FileNotFoundException(const std::string& filename) 
        : LoadException("File not found: " + filename) {}
};

class CorruptedSaveException : public LoadException {
public:
    explicit CorruptedSaveException(const std::string& details) 
        : LoadException("Corrupted save file: " + details) {}
};

class LevelException : public GameException {
public:
    explicit LevelException(const std::string& message) : GameException("Level error: " + message) {}
};

#endif