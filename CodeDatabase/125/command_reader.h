#pragma once
#include "command_handler.h"
#include <memory>

class CommandReader : public ICommandHandler {
public:
    std::unique_ptr<Command> GetPlayerCommand() override;
    void ClearInputBuffer() override;
};
