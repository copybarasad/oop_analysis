#pragma once
#include <fstream>
#include <stdexcept>
#include <string>

class FileOpenError : public std::runtime_error {
public:
    explicit FileOpenError(const std::string& msg);
};

class FileReadError : public std::runtime_error {
public:
    explicit FileReadError(const std::string& msg);
};

class InvalidSaveFormat : public std::runtime_error {
public:
    explicit InvalidSaveFormat(const std::string& msg);
};

class FileRAII {
private:
    std::fstream file_;
public:
    FileRAII(const std::string& filename, std::ios::openmode mode);
    ~FileRAII();

    std::fstream& get();
};