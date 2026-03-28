#ifndef COMMANDINPUTHANDLER_H
#define COMMANDINPUTHANDLER_H

#include "IInputHandler.h"
#include "ICommand.h"
#include <memory>
#include <string>

class CommandInputHandler : public IInputHandler {
public:
    std::unique_ptr<ICommand> getCommand() override;

private:
    char readCharInput();
    bool isValidCommandChar(char c);
    void displayAvailableCommands();
};

#endif // COMMANDINPUTHANDLER_H