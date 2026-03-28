#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <memory>
#include "Command.h"

// Interface for input handlers
class InputHandler {
public:
    virtual ~InputHandler() = default;
    virtual std::unique_ptr<Command> getNextCommand() = 0;
    virtual bool hasInput() const = 0;
};

#endif