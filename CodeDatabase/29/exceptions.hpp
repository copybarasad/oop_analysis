#pragma once
#include <stdexcept>
#include <string>

class GameError : public std::runtime_error {
public:
    explicit GameError(const std::string& message)
        : std::runtime_error(message) {}
};

class SaveFileError : public GameError {
public:
    explicit SaveFileError(const std::string& message)
        : GameError("Save file error: " + message) {}
};

class SaveFormatError : public GameError {
public:
    explicit SaveFormatError(const std::string& message)
        : GameError("Save format error: " + message) {}
};
