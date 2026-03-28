#ifndef CONSOLEINPUTHANDLER_H
#define CONSOLEINPUTHANDLER_H

#include "InputHandler.h"
#include "MoveUpCommand.h"
#include "MoveDownCommand.h"
#include "MoveLeftCommand.h"
#include "MoveRightCommand.h"
#include "SwitchModeCommand.h"
#include "QuitCommand.h"
#include <map>
#include <memory>

class ConsoleInputHandler : public InputHandler {
private:
    std::map<char, std::unique_ptr<Command>> keyBindings;
    
    void setupDefaultBindings();
    
public:
    ConsoleInputHandler();
    std::unique_ptr<Command> getNextCommand() override;
    bool hasInput() const override { return true; }
    
    void setBinding(char key, std::unique_ptr<Command> command);
};

#endif