#include "ConsoleInputHandler.h"
#include "MovementCommand.h"
#include "CastSpellCommand.h"
#include "SimpleCommand.h"
#include "CommandType.h"
#include <iostream>
#include <sstream>
#include <limits>

std::unique_ptr<Command> ConsoleInputHandler::getNextCommand() const {
    std::string input;
    std::cout << "> ";
    std::getline(std::cin, input);
    return parseInput(input);
}

std::unique_ptr<Command> ConsoleInputHandler::parseInput(const std::string& input) const {
    std::istringstream iss(input);
    std::string command;
    iss >> command;
    
    if (command == "w" || command == "up") {
        return std::make_unique<MovementCommand>(CommandType::MOVE_UP, 0, -1);
    } else if (command == "s" || command == "down") {
        return std::make_unique<MovementCommand>(CommandType::MOVE_DOWN, 0, 1);
    } else if (command == "a" || command == "left") {
        return std::make_unique<MovementCommand>(CommandType::MOVE_LEFT, -1, 0);
    } else if (command == "d" || command == "right") {
        return std::make_unique<MovementCommand>(CommandType::MOVE_RIGHT, 1, 0);
    } else if (command == "m") {
        return std::make_unique<SimpleCommand>(CommandType::SWITCH_COMBAT_MODE);
    } else if (command == "b") {
        return std::make_unique<SimpleCommand>(CommandType::BUY_SPELL);
    } else if (command == "save") {
        return std::make_unique<SimpleCommand>(CommandType::SAVE_GAME);
    } else if (command == "load") {
        return std::make_unique<SimpleCommand>(CommandType::LOAD_GAME);
    } else if (command == "quit" || command == "exit") {
        return std::make_unique<SimpleCommand>(CommandType::QUIT_GAME);
    } else if (command == "cast") {
        return parseSpellCommand();
    }
    
    return nullptr;
}

std::unique_ptr<Command> ConsoleInputHandler::parseSpellCommand() const {
    int spellIndex, x, y;
    
    std::cout << "Enter spell index: ";
    std::string indexStr;
    std::getline(std::cin, indexStr);
    std::istringstream indexIss(indexStr);
    
    if (!(indexIss >> spellIndex)) {
        std::cout << "Invalid spell index!\n";
        return nullptr;
    }
    
    std::cout << "Enter X coordinate: ";
    std::string xStr;
    std::getline(std::cin, xStr);
    std::istringstream xIss(xStr);
    
    if (!(xIss >> x)) {
        std::cout << "Invalid X coordinate!\n";
        return nullptr;
    }
    
    std::cout << "Enter Y coordinate: ";
    std::string yStr;
    std::getline(std::cin, yStr);
    std::istringstream yIss(yStr);
    
    if (!(yIss >> y)) {
        std::cout << "Invalid Y coordinate!\n";
        return nullptr;
    }
    
    return std::make_unique<CastSpellCommand>(spellIndex, Position(x, y));
}