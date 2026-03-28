#include "HelpCommand.h"
#include "CommandParser.h"
#include <iostream>

/**
 * @file HelpCommand.cpp
 * @brief Команда помощи (ОБНОВЛЕНО - динамические клавиши)
 */

bool HelpCommand::execute(Game* game) {
    if (keyConfig) {
        std::cout << CommandParser::getHelp(*keyConfig);
    } else {
        std::cout << CommandParser::getHelp();
    }
    return true;
}

std::string HelpCommand::getDescription() const {
    return "Show help";
}


