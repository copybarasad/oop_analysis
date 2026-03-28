#ifndef SAVEGAMEEXCEPTION_H
#define SAVEGAMEEXCEPTION_H

#include "GameException.h"

class SaveGameException : public GameException {
public:
    explicit SaveGameException(const std::string& msg) : GameException(msg) {}
};

#endif