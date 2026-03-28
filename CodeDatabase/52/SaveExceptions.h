#pragma once
#include <stdexcept>
#include <string>

class FileOpenException : public std::runtime_error {
public:
    FileOpenException(const std::string& msg)
        : std::runtime_error("FileOpenException: " + msg) {}
};

class SaveFormatException : public std::runtime_error {
public:
    SaveFormatException(const std::string& msg)
        : std::runtime_error("SaveFormatException: " + msg) {}
};