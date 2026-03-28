#ifndef GAME_SAVE_EXCEPTION_H_
#define GAME_SAVE_EXCEPTION_H_
#include <exception>
#include <string>

class GameSaveException : public std::exception {
public:
    explicit GameSaveException(const std::string& msg) : msg_(msg) {}
    const char* what() const noexcept override { return msg_.c_str(); }
private:
    std::string msg_;
};
#endif // GAME_SAVE_EXCEPTION_H_