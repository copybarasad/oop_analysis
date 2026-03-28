#pragma once
#include <stdexcept>

class FileOpenError : public std::runtime_error {
public:
    FileOpenError(const std::string& msg) : std::runtime_error(msg) {}
};

class InvalidSaveFormat : public std::runtime_error {
public:
    InvalidSaveFormat(const std::string& msg) : std::runtime_error(msg) {}
};

class CorruptedSaveError : public std::runtime_error {
public:
    CorruptedSaveError(const std::string& msg) : std::runtime_error(msg) {}
};