#pragma once
#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& message) : std::runtime_error(message) {}
};

class IOException : public GameException {
public:
    explicit IOException(const std::string& message) : GameException("IO Exception: " + message) {}
};

class InvalidStateException : public GameException {
public:
    explicit InvalidStateException(const std::string& message) : GameException("Invalid State Exception: " + message) {}
};

class InvalidUserInputException : public GameException {
public:
    explicit InvalidUserInputException(const std::string& message) : GameException("Invalid User Input: " + message) {}
};