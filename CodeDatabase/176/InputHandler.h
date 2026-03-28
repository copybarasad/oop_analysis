#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Command.h"
#include <memory>
#include <iostream>

class InputHandler {
public:
    InputHandler() = default;
    
    std::unique_ptr<Command> readInput(std::istream& inputStream) const;
    
    static bool isQuitCommand(const Command* command);
    
private:
    static Direction charToDirection(char input);
    std::unique_ptr<Command> createCommandFromChar(char input) const;
    std::unique_ptr<Command> createSpellCastCommand(char input, std::istream& inputStream) const;
};

#endif
