#ifndef GAMEEXCEPTIONS_H
#define GAMEEXCEPTIONS_H

#include <stdexcept>
#include <string>

// Base exception for all save/load errors
class SaveLoadException : public std::runtime_error {
public:
    explicit SaveLoadException(const std::string& message)
        : std::runtime_error("Save/Load Error: " + message) {}
};

class FileNotFoundError : public SaveLoadException {
public:
    explicit FileNotFoundError(const std::string& filename)
        : SaveLoadException("File not found: " + filename) {}
};

class CorruptedDataError : public SaveLoadException {
public:
    explicit CorruptedDataError(const std::string& details)
        : SaveLoadException("Corrupted save data: " + details) {}
};

class FileWriteError : public SaveLoadException {
public:
    explicit FileWriteError(const std::string& filename)
        : SaveLoadException("Cannot write to file: " + filename) {}
};

class InvalidSaveVersion : public SaveLoadException {
public:
    explicit InvalidSaveVersion(const std::string& version)
        : SaveLoadException("Invalid save version: " + version) {}
};

class SaveIntegrityError : public SaveLoadException {
public:
    explicit SaveIntegrityError(const std::string& reason)
        : SaveLoadException("Integrity check failed: " + reason) {}
};

#endif

