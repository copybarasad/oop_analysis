#ifndef SAVELOADEXCEPTIONS_H
#define SAVELOADEXCEPTIONS_H

#include <stdexcept>
#include <string>

class SaveLoadException : public std::runtime_error {
public:
    explicit SaveLoadException(const std::string& message)
        : std::runtime_error(message) {}
};

class FileAccessException : public SaveLoadException {
public:
    explicit FileAccessException(const std::string& filename, const std::string& operation)
        : SaveLoadException("Cannot " + operation + " file: " + filename) {}
};

class DataValidationException : public SaveLoadException {
public:
    explicit DataValidationException(const std::string& field, const std::string& details)
        : SaveLoadException("Data validation failed for " + field + ": " + details) {}
};

class JSONParseException : public SaveLoadException {
public:
    explicit JSONParseException(const std::string& details)
        : SaveLoadException("JSON parsing error: " + details) {}
};

#endif // SAVELOADEXCEPTIONS_H