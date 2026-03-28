#ifndef CONSOLE_INPUT_HANDLER_H
#define CONSOLE_INPUT_HANDLER_H

#include "Command.h"
#include <memory>

class ConsoleInputHandler {
public:
    std::unique_ptr<Command> getNextCommand();
    
private:
    std::unique_ptr<Command> parseInput(const std::string& input);
};

#endif