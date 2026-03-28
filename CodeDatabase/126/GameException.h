#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    GameException(const std::string& message) : std::runtime_error(message) {}
};

class SaveGameException : public GameException {
    std::string filename;
public:
    SaveGameException(const std::string& filename, const std::string& message)
        : GameException("Save error: " + message), filename(filename) {}
};

class LoadGameException : public GameException {
public:
    LoadGameException(const std::string& message) : GameException("Load error: " + message) {}
};

class FileNotFoundException : public LoadGameException {
public:
    FileNotFoundException(const std::string& filename)
        : LoadGameException("File not found: " + filename) {}
};

class CorruptedSaveException : public LoadGameException {
public:
    CorruptedSaveException(const std::string& filename, const std::string& details)
        : LoadGameException("Corrupted save file " + filename + ": " + details) {}
};

#endif