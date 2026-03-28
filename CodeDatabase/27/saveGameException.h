#pragma once
#include <stdexcept>
#include <string>

class SaveGameException : public std::runtime_error {
public:
    explicit SaveGameException(const std::string& msg)
        : std::runtime_error("SaveGameException: " + msg) {}
};