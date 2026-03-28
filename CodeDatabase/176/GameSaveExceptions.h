#ifndef GAME_SAVE_EXCEPTIONS_H
#define GAME_SAVE_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class SaveException : public std::runtime_error {
public:
    explicit SaveException(const std::string& message) 
        : std::runtime_error("Save Error: " + message) {}
};

class FileOpenException : public SaveException {
public:
    explicit FileOpenException(const std::string& filename) 
        : SaveException("Cannot open file: " + filename) {}
};

class FileWriteException : public SaveException {
public:
    explicit FileWriteException(const std::string& filename) 
        : SaveException("Cannot write to file: " + filename) {}
};

class FileReadException : public SaveException {
public:
    explicit FileReadException(const std::string& filename) 
        : SaveException("Cannot read from file: " + filename) {}
};

class FileCorruptedException : public SaveException {
public:
    explicit FileCorruptedException(const std::string& filename) 
        : SaveException("File is corrupted: " + filename) {}
};

class DataValidationException : public SaveException {
public:
    explicit DataValidationException(const std::string& details) 
        : SaveException("Invalid save data: " + details) {}
};

class SaveNotFoundException : public SaveException {
public:
    explicit SaveNotFoundException(const std::string& filename) 
        : SaveException("Save file not found: " + filename) {}
};

#endif
