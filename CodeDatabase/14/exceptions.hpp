#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

class GameException : public std::exception {
protected:
    std::string message;
public:
    explicit GameException(const std::string& msg) : message(msg) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

class SaveException : public GameException {
public:
    explicit SaveException(const std::string& msg) : GameException("Save Error: " + msg) {}
};

class LoadException : public GameException {
public:
    explicit LoadException(const std::string& msg) : GameException("Load Error: " + msg) {}
};

#endif