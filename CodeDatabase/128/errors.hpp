#pragma once
#include <stdexcept>
#include <string>

class ExceptionsForSaveLoad : public std::runtime_error {
public:
    explicit ExceptionsForSaveLoad(const std::string& msg)
        : std::runtime_error(msg) {}
};

class FileNotExistError : public ExceptionsForSaveLoad {
public:
    explicit FileNotExistError(const std::string& path)
        : ExceptionsForSaveLoad("Sorry, file not found: " + path) {}
};

class CorruptedDataError : public ExceptionsForSaveLoad {
public:
    explicit CorruptedDataError(const std::string& msg)
        : ExceptionsForSaveLoad("Sorry, invalid data: " + msg) {}
};

class LoadError : public ExceptionsForSaveLoad {
public:
    explicit LoadError(const std::string& msg)
        : ExceptionsForSaveLoad("Sorry, load error: " + msg) {}
};

class SaveError : public ExceptionsForSaveLoad {
public:
    explicit SaveError(const std::string& msg)
        : ExceptionsForSaveLoad("Sorry, save error: " + msg) {}
};
