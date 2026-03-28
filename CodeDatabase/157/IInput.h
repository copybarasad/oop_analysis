#ifndef IINPUT_H
#define IINPUT_H

#include "Command.h"

class IInput {
public:
    virtual ~IInput() = default;
    virtual Command nextCommand() = 0;
};

#endif // IINPUT_H
