#pragma once
#include <stdexcept>
#include <string>

class LoadGameException : public std::runtime_error {
public:
    explicit LoadGameException(const std::string& msg)
        : std::runtime_error("LoadGameException: " + msg) {}
};