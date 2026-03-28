#ifndef INPUT_SYSTEM_H
#define INPUT_SYSTEM_H

#include "Command.h"
#include <string>
#include <unordered_map>
#include <iostream>  
#include <limits>    

class InputSystem {
private:
    std::unordered_map<std::string, Command> commandMap;
    void initializeCommandMap();

public:
    InputSystem();
    Command processInput(const std::string& input);
    
    Command getCommand() {
        std::string input;
        std::getline(std::cin, input);
        return processInput(input);
    }
};

#endif