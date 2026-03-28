#include "ConsoleInputProcessor.h"
#include <iostream>
#include <cctype>

std::unique_ptr<Command> ConsoleInputProcessor::processInput() {
    char input = getRawInput();
    return convertToCommand(input);
}

char ConsoleInputProcessor::getRawInput() {
    char input;
    while (true) {
        std::cin >> input;
        input = std::toupper(input);
        
        if (input == 'W' || input == 'A' || input == 'S' || input == 'D' || 
            input == 'Q' || input == 'C' || input == 'P') {
            std::cin.ignore(10000, '\n');
            return input;
        } else {
            std::cout << "Invalid input! Use only: W, A, S, D, Q, C, P" << std::endl;
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }
}

std::unique_ptr<Command> ConsoleInputProcessor::convertToCommand(char input) {
    switch (input) {
        case 'W': return std::make_unique<MoveCommand>(0, -1);
        case 'S': return std::make_unique<MoveCommand>(0, 1);
        case 'A': return std::make_unique<MoveCommand>(-1, 0);
        case 'D': return std::make_unique<MoveCommand>(1, 0);
        case 'Q': return std::make_unique<SwitchCombatModeCommand>();
        case 'C': return std::make_unique<CastSpellCommand>();
        case 'P': return std::make_unique<PauseCommand>();
        default: return nullptr;
    }
}