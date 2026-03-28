#pragma once
#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    GameException(const std::string& msg) : std::runtime_error(msg) {}
};

class FileOpenException : public GameException {
public:
    FileOpenException(const std::string& filename) 
        : GameException("Error: Could not open file '" + filename + "'") {}
};

class CorruptDataException : public GameException {
public:
    CorruptDataException(const std::string& details) 
        : GameException("Error: Save data is corrupt. " + details) {}
};

class SaveException : public GameException {
public:
    SaveException(const std::string& details) 
        : GameException("Error saving game: " + details) {}
};