#pragma once
#include <memory>
#include "Command.h"

class ConsoleInputHandler {
public:
    std::unique_ptr<Command> getCommand();
};