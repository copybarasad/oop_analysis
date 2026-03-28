#ifndef SAVEERROR_H
#define SAVEERROR_H
#include <stdexcept>
#include <string>
#include "gameException.h"

class SaveDataError : public GameException {
public:
    explicit SaveDataError(const std::string& details) 
        : GameException(details) {}
};
#endif