#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <exception>
#include <string>

class GameException : public std::exception {
private:
    std::string message;
public:
    explicit GameException(const std::string& msg) : message(msg) {}
    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

class SaveGameException : public GameException {
public:
    SaveGameException(const std::string& msg) : GameException("Save error: " + msg) {}
};

class LoadGameException : public GameException {
public:
    LoadGameException(const std::string& msg) : GameException("Load error: " + msg) {}
};

#endif