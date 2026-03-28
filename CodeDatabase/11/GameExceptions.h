#ifndef GAMEEXCEPTIONS_H
#define GAMEEXCEPTIONS_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message) : std::runtime_error(message) {}
};

class FileOpenException : public GameException {
public:
    explicit FileOpenException(const std::string& filename) 
        : GameException("Error: Could not open file '" + filename + "'.") {}
};

class FileFormatException : public GameException {
public:
    explicit FileFormatException(const std::string& details) 
        : GameException("Error: Corrupted save file or invalid format. " + details) {}
};

#endif