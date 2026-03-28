#ifndef COMMAND_H
#define COMMAND_H

#include "CommandType.h"

class Command {
public:
    virtual ~Command() = default;
    virtual CommandType getType() const = 0;
    virtual bool isValid() const = 0;
};

#endif
