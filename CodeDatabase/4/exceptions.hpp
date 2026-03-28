#pragma once

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& msg) : std::runtime_error("Game Error: " + msg) {}
};

class SaveLoadException : public GameException {
public:
    explicit SaveLoadException(const std::string& msg) : GameException("Save/Load Error: " + msg) {}
};

class FileIOException : public SaveLoadException {
public:
    explicit FileIOException(const std::string& msg) : SaveLoadException("File I/O Error: " + msg) {}
};

class InvalidDataException : public SaveLoadException {
public:
    explicit InvalidDataException(const std::string& msg) : SaveLoadException("Invalid Data Error: " + msg) {}
};