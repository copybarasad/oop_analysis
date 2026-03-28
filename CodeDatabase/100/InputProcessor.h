#pragma once
#include "Command.h"
#include <memory>

class InputProcessor {
public:
    virtual ~InputProcessor() = default;
    virtual std::unique_ptr<Command> processInput() = 0;
};