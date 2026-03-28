#pragma once
#include "../commands/Command.h"
#include "GameCycle.h"
#include <memory>

class InputReader {
public:
    virtual ~InputReader() = default;
    virtual std::unique_ptr<Command> readCommand(const GameCycle& game) = 0;
};
