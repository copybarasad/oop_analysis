#pragma once
#include <stdexcept>
#include <string>

class InvalidSaveDataException : public std::runtime_error {
public:
    explicit InvalidSaveDataException(const std::string& msg)
        : std::runtime_error("InvalidSaveDataException: " + msg) {}
};
