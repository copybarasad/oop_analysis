#ifndef GAMEEXCEPTIONS_H
#define GAMEEXCEPTIONS_H

#include <stdexcept>
#include <string>

// Base game exception
class GameException : public std::runtime_error {
public:
    GameException(const std::string& message) : std::runtime_error(message) {}
};

// File-related exceptions
class FileOpenException : public GameException {
public:
    FileOpenException(const std::string& message) : GameException(message) {}
};

class FileReadException : public GameException {
public:
    FileReadException(const std::string& message) : GameException(message) {}
};

class FileWriteException : public GameException {
public:
    FileWriteException(const std::string& message) : GameException(message) {}
};

// Save/Load specific exceptions
class SaveGameException : public GameException {
public:
    SaveGameException(const std::string& message) : GameException(message) {}
};

class LoadGameException : public GameException {
public:
    LoadGameException(const std::string& message) : GameException(message) {}
};

// Game state exceptions
class InvalidGameStateException : public GameException {
public:
    InvalidGameStateException(const std::string& message) : GameException(message) {}
};

class InvalidSaveFormatException : public LoadGameException {
public:
    InvalidSaveFormatException(const std::string& filename, const std::string& details) 
        : LoadGameException("Invalid save format in " + filename + ": " + details) {}
};

class FileNotFoundException : public LoadGameException {
public:
    FileNotFoundException(const std::string& filename) 
        : LoadGameException("Save file not found: " + filename) {}
};

class CorruptedSaveException : public LoadGameException {
public:
    CorruptedSaveException(const std::string& filename, const std::string& section) 
        : LoadGameException("Corrupted save file " + filename + " in section: " + section) {}
};

#endif