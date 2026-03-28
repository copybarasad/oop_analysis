#include "InputHandler.h"
#include <iostream>
#include <cctype>

Direction InputHandler::charToDirection(char input) {
    switch (std::tolower(input)) {
        case 'w': return Direction::UP;
        case 's': return Direction::DOWN;
        case 'a': return Direction::LEFT;
        case 'f': return Direction::RIGHT;
        case 'i': return Direction::UP;
        case 'k': return Direction::DOWN;
        case 'j': return Direction::LEFT;
        case 'l': return Direction::RIGHT;
        default: throw std::invalid_argument("Invalid direction character");
    }
}

std::unique_ptr<Command> InputHandler::createCommandFromChar(char input) const {
    char lowerInput = std::tolower(input);
    
    if (lowerInput == 'w' || lowerInput == 's' ||
        lowerInput == 'a' || lowerInput == 'f') {
        Direction dir = charToDirection(lowerInput);
        return std::make_unique<MoveCommand>(dir);
    }
    
    if (lowerInput == 'i' || lowerInput == 'k' ||
        lowerInput == 'j' || lowerInput == 'l') {
        Direction dir = charToDirection(lowerInput);
        return std::make_unique<AttackCommand>(dir);
    }
    
    if (lowerInput == 'm') {
        return std::make_unique<SwitchModeCommand>();
    }
    
    if (lowerInput == 'p') {
        return std::make_unique<PurchaseSpellCommand>();
    }
    
    if (lowerInput == 'o') {
        return std::make_unique<SaveGameCommand>();
    }
    
    return nullptr;
}

std::unique_ptr<Command> InputHandler::createSpellCastCommand(char input, std::istream& inputStream) const {
    if (!std::isdigit(input) || input < '1' || input > '6') {
        return nullptr;
    }
    
    int spellIndex = input - '1';
    
    std::cout << "Enter target coordinates (x y): ";
    int targetX, targetY;
    
    if (inputStream >> targetX >> targetY) {
        inputStream.ignore(10000, '\n');
        return std::make_unique<CastSpellCommand>(spellIndex, targetX, targetY);
    } else {
        inputStream.clear();
        inputStream.ignore(10000, '\n');
        std::cout << "Invalid coordinates!\n";
        return nullptr;
    }
}

std::unique_ptr<Command> InputHandler::readInput(std::istream& inputStream) const {
    char input;
    inputStream >> input;
    inputStream.ignore(10000, '\n');
    
    if (std::isdigit(input) && input >= '1' && input <= '6') {
        auto cmd = createSpellCastCommand(input, inputStream);
        if (cmd) return cmd;
    }
    
    if (std::tolower(input) == 'q') {
        return std::make_unique<QuitCommand>();
    }
    
    return createCommandFromChar(input);
}

bool InputHandler::isQuitCommand(const Command* command) {
    return dynamic_cast<const QuitCommand*>(command) != nullptr;
}
