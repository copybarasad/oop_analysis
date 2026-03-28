#ifndef GAME_LOAD_EXCEPTION_H_
#define GAME_LOAD_EXCEPTION_H_
#include <exception>
#include <string>

class GameLoadException : public std::exception {
public:
    explicit GameLoadException(const std::string& msg) : msg_(msg) {}
    const char* what() const noexcept override { return msg_.c_str(); }
private:
    std::string msg_;
};
#endif // GAME_LOAD_EXCEPTION_H_