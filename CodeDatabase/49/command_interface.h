#ifndef COMMAND_INTERFACE_H
#define COMMAND_INTERFACE_H

#include "command_type.h"

class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual CommandType get_type() const = 0;
};

#endif