#ifndef SAVELOADEXCEPTION_H
#define SAVELOADEXCEPTION_H

#include <stdexcept>
#include <string>

class SaveLoadException : public std::runtime_error {
public:
    SaveLoadException(const std::string& message) : std::runtime_error(message) {}
};

class SaveException : public SaveLoadException {
public:
    SaveException(const std::string& message) : SaveLoadException("Save error: " + message) {}
};

class LoadException : public SaveLoadException {
public:
    LoadException(const std::string& message) : SaveLoadException("Load error: " + message) {}
};

class CorruptedSaveException : public LoadException {
public:
    CorruptedSaveException(const std::string& filename, const std::string& details) 
        : LoadException("save file " + filename + " was corrupted. Details: " + details) {}
};

class ValidationException : public LoadException {
public:
    ValidationException(const std::string& filename): LoadException("save file " + filename + " was modified by an outsider") {}
};

#endif