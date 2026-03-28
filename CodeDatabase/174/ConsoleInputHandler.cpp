#include "ConsoleInputHandler.h"
#include <iostream>
#include <sstream>
#include <string>
#include <conio.h>

std::unique_ptr<Command> ConsoleInputHandler::getNextCommand() {
    std::cout << "\nYour move: ";
    char input = _getch();
    std::cout << input << std::endl;
    
    return parseInput(std::string(1, input));
}

std::unique_ptr<Command> ConsoleInputHandler::parseInput(const std::string& input) {
    if (input.empty()) {
        return std::make_unique<Command>(CommandType::UNKNOWN);
    }
    
    char command = input[0];
    
    switch (command) {
        case 'w': case 'W':
            return std::make_unique<Command>(CommandType::MOVE_UP);
        case 's': case 'S':
            return std::make_unique<Command>(CommandType::MOVE_DOWN);
        case 'a': case 'A':
            return std::make_unique<Command>(CommandType::MOVE_LEFT);
        case 'd': case 'D':
            return std::make_unique<Command>(CommandType::MOVE_RIGHT);
        case 'f': case 'F':
            return std::make_unique<Command>(CommandType::ATTACK);
        case 'm': case 'M':
            return std::make_unique<Command>(CommandType::CAST_SPELL);
        case 'c': case 'C':
            return std::make_unique<Command>(CommandType::CHANGE_STYLE);
        case 'o': case 'O':
            return std::make_unique<Command>(CommandType::SAVE_GAME);
        case 'l': case 'L':
            return std::make_unique<Command>(CommandType::LOAD_GAME);
        case 'q': case 'Q':
            return std::make_unique<Command>(CommandType::QUIT);
        default:
            return std::make_unique<Command>(CommandType::UNKNOWN);
    }
}