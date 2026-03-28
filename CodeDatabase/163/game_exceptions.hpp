#ifndef GAME_EXCEPTIONS_HPP
#define GAME_EXCEPTIONS_HPP

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& msg) : std::runtime_error(msg) {}
};

class SaveGameException : public GameException {
public:
    explicit SaveGameException(const std::string& msg) : GameException("Save Error: " + msg) {}
};

class LoadGameException : public GameException {
public:
    explicit LoadGameException(const std::string& msg) : GameException("Load Error: " + msg) {}
};

class FileNotFoundException : public LoadGameException {
public:
    explicit FileNotFoundException(const std::string& filename) 
        : LoadGameException("File not found: " + filename) {}
};

class CorruptSaveException : public LoadGameException {
public:
    explicit CorruptSaveException(const std::string& details) 
        : LoadGameException("Corrupted save file: " + details) {}
};

class WriteFileException : public SaveGameException {
public:
    explicit WriteFileException(const std::string& filename) 
        : SaveGameException("Cannot write to file: " + filename) {}
};

#endif