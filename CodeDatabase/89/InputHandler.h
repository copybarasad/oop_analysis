#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include "Command.h"
#include <iostream>
#include <memory>
#include <string>
#include <limits>
#include <cctype>

class IInputHandler {
public:
    virtual ~IInputHandler() = default;
    virtual std::unique_ptr<Command> getCommand() = 0;
    virtual void setMode(bool isMenuMode) = 0;  
};

class ConsoleInputHandler : public IInputHandler {
public:
    ConsoleInputHandler() : menuMode(false) {}
    std::unique_ptr<Command> getCommand() override;
    void setMode(bool isMenuMode) override { menuMode = isMenuMode; }
    
private:
    Direction getDirectionFromChar(char ch) const;
    std::unique_ptr<Command> handleGameInput(char input);
    std::unique_ptr<Command> handleMenuInput(char input);
    bool menuMode;
};

#endif