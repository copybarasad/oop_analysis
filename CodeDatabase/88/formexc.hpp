#ifndef F_EXC_HPP
#define F_EXC_HPP
#include "gameexc.hpp"

class FileFormatException : public GameException {
public:
    explicit FileFormatException(const std::string& msg)
        : GameException("Format error: " + msg) {}
};

#endif