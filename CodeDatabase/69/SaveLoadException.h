#pragma once
#include <stdexcept>
#include <string>

class SaveException : public std::runtime_error {
public:
    explicit SaveException(const std::string& msg);
};

class LoadException : public std::runtime_error {
public:
    explicit LoadException(const std::string& msg);
};