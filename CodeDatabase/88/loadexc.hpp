#ifndef L_EXC_HPP
#define L_EXC_HPP
#include "gameexc.hpp"

class LoadException : public GameException {
public:
    explicit LoadException(const std::string& msg)
        : GameException("Load error: " + msg) {}
};

#endif