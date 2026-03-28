// ICommandSource.h
#pragma once
#include "Command.h"

class ICommandSource {
public:
    virtual ~ICommandSource() = default;
    virtual Command GetCommand() = 0;
};
