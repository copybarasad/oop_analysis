#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message)
        : std::runtime_error(message) {}
};

class SaveGameException : public GameException {
public:
    explicit SaveGameException(const std::string& message)
        : GameException("Save failed: " + message) {}
};

class LoadGameException : public GameException {
public:
    explicit LoadGameException(const std::string& message)
        : GameException("Load failed: " + message) {}
};

class FileNotFoundException : public LoadGameException {
public:
    explicit FileNotFoundException(const std::string& filename)
        : LoadGameException("File not found: " + filename) {}
};

class CorruptedSaveException : public LoadGameException {  // ДОБАВЛЯЕМ ЭТОТ КЛАСС
public:
    explicit CorruptedSaveException(const std::string& details)
        : LoadGameException("Corrupted save file: " + details) {}
};

#endif
