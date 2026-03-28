#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& msg) : std::runtime_error(msg) {}
};

class FileIOException : public GameException {
public:
    explicit FileIOException(const std::string& msg) 
        : GameException(std::string("File I/O error: ") + msg) {}
};

class SaveException : public GameException {
public:
    explicit SaveException(const std::string& msg) 
        : GameException(std::string("Save error: ") + msg) {}
};

class LoadException : public GameException {
public:
    explicit LoadException(const std::string& msg) 
        : GameException(std::string("Load error: ") + msg) {}
};

class ParseException : public GameException {
public:
    explicit ParseException(const std::string& msg) 
        : GameException(std::string("Parse error: ") + msg) {}
};

class GameLogicException : public GameException {
public:
    explicit GameLogicException(const std::string& msg) 
        : GameException(std::string("Game logic error: ") + msg) {}
};

class InvalidOperationException : public GameException {
public:
    explicit InvalidOperationException(const std::string& msg) 
        : GameException(std::string("Invalid operation: ") + msg) {}
};

class ResourceException : public GameException {
public:
    explicit ResourceException(const std::string& msg) 
        : GameException(std::string("Resource error: ") + msg) {}
};

#endif