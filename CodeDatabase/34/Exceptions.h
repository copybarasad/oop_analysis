#ifndef GAME_EXCEPTIONS_H
#define GAME_EXCEPTIONS_H

#include <stdexcept>
#include <string>

namespace Game {

class SaveLoadException : public std::runtime_error {
public:
    explicit SaveLoadException(const std::string& message) : std::runtime_error(message) {}
};

class FileOpenException : public SaveLoadException {
public:
    explicit FileOpenException(const std::string& filename)
        : SaveLoadException("Error: Could not open file '" + filename + "' for reading or writing.") {}
};

class FileReadException : public SaveLoadException {
public:
    explicit FileReadException(const std::string& details)
        : SaveLoadException("Error: Failed to read from save file. Details: " + details) {}
};

class FileWriteException : public SaveLoadException {
public:
    explicit FileWriteException(const std::string& details)
        : SaveLoadException("Error: Failed to write to save file. Details: " + details) {}
};

class InvalidDataException : public SaveLoadException {
public:
    explicit InvalidDataException(const std::string& details)
        : SaveLoadException("Error: Save file contains invalid or corrupted data. Details: " + details) {}
};

}

#endif