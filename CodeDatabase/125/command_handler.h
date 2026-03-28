#pragma once
#include "command.h" 
#include <memory>

class ICommandHandler {
public:
    virtual ~ICommandHandler() = default;
    virtual std::unique_ptr<Command> GetPlayerCommand() = 0;
    virtual void ClearInputBuffer() = 0;
};
