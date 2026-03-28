#pragma once
#include <stdexcept>
#include <string>

class GameError : public std::runtime_error {
public:
    explicit GameError(const std::string& msg) : std::runtime_error(msg) {}
};

class FileOpenError : public GameError {
public:
    explicit FileOpenError(const std::string& path)
        : GameError("FileOpenError: cannot open file: " + path) {}
};

class FileWriteError : public GameError {
public:
    explicit FileWriteError(const std::string& path)
        : GameError("FileWriteError: cannot write file: " + path) {}
};

class FileReadError : public GameError {
public:
    explicit FileReadError(const std::string& path)
        : GameError("FileReadError: cannot read file: " + path) {}
};

class ParseError : public GameError {
public:
    explicit ParseError(const std::string& what)
        : GameError("ParseError: " + what) {}
};

class BadHeaderError : public GameError {
public:
    explicit BadHeaderError(const std::string& msg)
        : GameError("BadHeaderError: " + msg) {}
};
