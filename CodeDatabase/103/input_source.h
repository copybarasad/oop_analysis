#pragma once
#include "direction.h"
#include <string>

class IInputSource {
public:
    virtual ~IInputSource() = default;
    virtual Direction getDirection() = 0;
    virtual std::string getString() = 0;
    virtual int getInt() = 0;
    virtual char getChar() = 0;
    virtual char getCharImmediate() = 0;
};