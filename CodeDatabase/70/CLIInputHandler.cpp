#include "CLIInputHandler.h"

CLIInputHandler::CLIInputHandler()
{
    CLIConfigurator configurator;
    commandMap = configurator.readCFG();
}

Command CLIInputHandler::getCommand()
{
    bool isValidCommand = false;
    Command command;
    char inputKey;

    while (!isValidCommand) 
    {
        std::cout << "Your turn: ";
        inputKey = getch();
        if (commandMap.count(inputKey)) 
        {
            command.setCommandType(commandMap[inputKey]);
            isValidCommand = true;
        } else {
            std::cout << "Error: Unknown command '" << inputKey << "'. Please try again.\n";
        }
    }
    return command;
}