#ifndef LOADGAMEEXCEPTION_H
#define LOADGAMEEXCEPTION_H

#include "GameException.h"

class LoadGameException : public GameException {
public:
    explicit LoadGameException(const std::string& msg) : GameException(msg) {}
};

#endif