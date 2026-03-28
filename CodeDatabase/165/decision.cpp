#include "decision.h"
#include <iostream>
#include <string>

size_t decision::getANumber()
{
    std::string command;
    std::cout << "Make a choice: ";
    std::getline(std::cin, command);
    if(!command.size()){
        return 0;
    }
    for(char x : command){
        if(!isdigit(x)){
            std::cout << "Wrong input\n";
            return 0;
        }
    }
    return std::stoull(command);
}

char decision::getACharacter()
{
    std::string command;
    std::cout << "Enter a character: ";
    std::getline(std::cin, command);
    return command[0];
}

std::string decision::readLine()
{
    std::string command;
    std::cout << "Enter command: ";
    std::getline(std::cin, command);
    return command;
}