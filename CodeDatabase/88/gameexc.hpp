#ifndef G_EXC_HPP
#define G_EXC_HPP
#include <stdexcept>

class GameException : public std::runtime_error {
public:
    explicit GameException(const std::string& msg)
        : std::runtime_error(msg) {}
};

#endif