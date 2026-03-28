#include "InputHandler.h"
#include <iostream>

Direction ConsoleInputHandler::getDirectionFromChar(char ch) const {
    ch = std::toupper(ch);
    switch (ch) {
        case 'W': return Direction::Up;
        case 'S': return Direction::Down;
        case 'A': return Direction::Left;
        case 'D': return Direction::Right;
        default: return Direction::Up;
    }
}

std::unique_ptr<Command> ConsoleInputHandler::handleGameInput(char input) {
    input = std::toupper(input);
    
    if (input == 'W' || input == 'S' || input == 'A' || input == 'D') {
        return std::make_unique<Command>(MoveCommand{getDirectionFromChar(input)});
    }
    else if (input == 'F') {
        std::cout << "Direction (W/A/S/D): ";
        char dirChar;
        std::cin >> dirChar;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::make_unique<Command>(AttackCommand{getDirectionFromChar(dirChar)});
    }
    else if (input == 'C') {
        std::cout << "Spell index: ";
        int idx;
        std::cin >> idx;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Direction (W/A/S/D): ";
        char dirChar;
        std::cin >> dirChar;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::make_unique<Command>(CastSpellCommand{idx, getDirectionFromChar(dirChar)});
    }
    else if (input == 'X') {
        return std::make_unique<Command>(SwitchModeCommand{});
    }
    else if (input == 'V') {
        return std::make_unique<Command>(ViewSpellsCommand{});
    }
    else if (input == 'Z') {
        std::cout << "Enter save filename (without .txt): ";
        std::string filename;
        std::cin >> filename;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (filename.length() < 4 || filename.substr(filename.length()-4) != ".txt") {
            filename += ".txt";
        }
        return std::make_unique<Command>(SaveGameCommand{filename});
    }
    else if (input == 'Q') {
        return std::make_unique<Command>(QuitCommand{});
    }
    else if (input == 'L') {
        std::cout << "Enter load filename: ";
        std::string filename;
        std::cin >> filename;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return std::make_unique<Command>(LoadGameCommand{filename});
    }
    
    return nullptr;
}

std::unique_ptr<Command> ConsoleInputHandler::handleMenuInput(char input) {
    switch (input) {
        case '1': return std::make_unique<Command>(NextLevelCommand{});
        case '2': {
            std::cout << "Enter save filename to load: ";
            std::string filename;
            std::cin >> filename;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return std::make_unique<Command>(LoadGameCommand{filename});
        }
        case '3': return std::make_unique<Command>(QuitCommand{});
        default: return nullptr;
    }
}

std::unique_ptr<Command> ConsoleInputHandler::getCommand() {
    if (menuMode) {
        std::cout << "Select option (1-3): ";
    } else {
        std::cout << "\nEnter action (WASD to move, F-attack, C-cast spell, X-switch mode, V-view spells, Z-save, Q-quit): ";
    }
    
    char input;
    std::cin >> input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    if (menuMode) {
        return handleMenuInput(input);
    } else {
        return handleGameInput(input);
    }
}