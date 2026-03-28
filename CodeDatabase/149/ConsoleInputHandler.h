#ifndef CONSOLEINPUTHANDLER_H
#define CONSOLEINPUTHANDLER_H

#include "Command.h"
#include <memory>
#include <string>

class ConsoleInputHandler {
public:
    std::unique_ptr<Command> getNextCommand() const;
    
private:
    std::unique_ptr<Command> parseInput(const std::string& input) const;
    std::unique_ptr<Command> parseSpellCommand() const;
};

#endif