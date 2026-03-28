#pragma once
#include <exception>
#include <string>

class GameSaveException : public std::exception {
private:
    std::string message_;

public:
    explicit GameSaveException(const std::string& message) : message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }
};

class GameLoadException : public std::exception {
private:
    std::string message_;

public:
    explicit GameLoadException(const std::string& message) : message_(message) {}

    const char* what() const noexcept override {
        return message_.c_str();
    }
};