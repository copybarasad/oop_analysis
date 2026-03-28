#ifndef GAMEEXCEPTION_H
#define GAMEEXCEPTION_H

#include <exception>
#include <string>

/**
 * @brief Базовое исключение для игровых ошибок
 */
class GameException : public std::exception {
protected:
    std::string message;
    
public:
    explicit GameException(const std::string& msg) : message(msg) {}
    
    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif // GAMEEXCEPTION_H


