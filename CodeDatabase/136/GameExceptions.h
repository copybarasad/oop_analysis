#ifndef GAMEEXCEPTIONS_H
#define GAMEEXCEPTIONS_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    GameException(const std::string& message, const std::string& context = "")
        : std::runtime_error("Game Error: " + message + (context.empty() ? "" : " [" + context + "]")) {}
};

class FileException : public GameException {
public:
    FileException(const std::string& filename, const std::string& operation)
        : GameException("File operation failed: " + operation, "File: " + filename) {}
};

class LoadException : public GameException {
public:
    LoadException(const std::string& section, const std::string& details = "")
        : GameException("Load failed in section: " + section, details) {}
};

class SaveException : public GameException {
public:
    SaveException(const std::string& section, const std::string& details = "")
        : GameException("Save failed in section: " + section, details) {}
};

class ValidationException : public GameException {
public:
    ValidationException(const std::string& dataType, const std::string& value = "")
        : GameException("Data validation failed for: " + dataType, value) {}
};

class InitializationException : public GameException {
public:
    InitializationException(const std::string& component, const std::string& details = "")
        : GameException("Initialization failed for: " + component, details) {}
};

class GameLogicException : public GameException {
public:
    GameLogicException(const std::string& operation, const std::string& details = "")
        : GameException("Game logic error in: " + operation, details) {}
};

#endif