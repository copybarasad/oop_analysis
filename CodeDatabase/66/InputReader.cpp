#include "../include/InputReader.h"
#include "../include/SpellHand.h"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

std::string ConsoleInputHandler::readLine() {
    std::string line;
    std::getline(std::cin, line);
    return line;
}

char ConsoleInputHandler::readChar() {
    std::string line = readLine();
    
    size_t start = line.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return ' ';
    }
    
    return line[start];
}

Direction ConsoleInputHandler::parseDirectionInput(KeyBindingConfig& keyBindings) {
    std::cout << "Enter direction: ";
    char dirChar = readChar();
    return keyBindings.getDirectionForKey(dirChar);
}

Position ConsoleInputHandler::parsePositionInput() {
    std::cout << "Enter target X Y: ";
    std::string line = readLine();
    std::istringstream iss(line);
    int x, y;
    if (iss >> x >> y) {
        return Position(x, y);
    }
    return Position(-1, -1);
}

int ConsoleInputHandler::parseSpellIndex(const SpellHand* spellHand) {
    if (!spellHand || spellHand->getSpellCount() == 0) {
        return -1;
    }
    
    std::cout << "Enter spell number (1-" << spellHand->getSpellCount() << "): ";
    std::string line = readLine();
    
    size_t start = line.find_first_not_of(" \t");
    size_t end = line.find_last_not_of(" \t");
    if (start == std::string::npos) {
        return -1;
    }
    std::string trimmed = line.substr(start, end - start + 1);
    
    for (char c : trimmed) {
        if (!std::isdigit(c)) {
            return -1;
        }
    }
    
    try {
        int index = std::stoi(trimmed);
        if (index >= 1 && index <= spellHand->getSpellCount()) {
            return index - 1;
        }
    } catch (...) {
        return -1;
    }
    
    return -1;
}

std::unique_ptr<Command> ConsoleInputHandler::readAndConvert(KeyBindingConfig& keyBindings,
                                                              const SpellHand* spellHand) {
    std::cout << "Enter command: ";
    std::string input = readLine();
    
    size_t start = input.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return std::make_unique<SkipTurnCommand>();
    }
    
    size_t end = input.find_last_not_of(" \t\n\r");
    std::string trimmed = input.substr(start, end - start + 1);
    
    if (trimmed == "space" || trimmed == "SPACE") {
        return std::make_unique<SkipTurnCommand>();
    }
    
    if (trimmed == "save" || trimmed == "SAVE") {
        return std::make_unique<SaveGameCommand>();
    }
    
    if (trimmed.length() != 1) {
        return std::make_unique<InvalidCommand>("Invalid input: use single character commands");
    }
    
    char key = trimmed[0];
    CommandType cmdType = keyBindings.getCommandForKey(key);
    
    switch (cmdType) {
        case CommandType::MOVE: {
            Direction dir = keyBindings.getDirectionForKey(key);
            if (dir != Direction::NONE) {
                return std::make_unique<MoveCommand>(dir);
            }
            return std::make_unique<InvalidCommand>("Invalid direction");
        }
        
        case CommandType::ATTACK: {
            Direction dir = parseDirectionInput(keyBindings);
            if (dir != Direction::NONE) {
                return std::make_unique<AttackCommand>(dir);
            }
            return std::make_unique<InvalidCommand>("Invalid attack direction");
        }
        
        case CommandType::SWITCH_MODE:
            return std::make_unique<SwitchModeCommand>();
        
        case CommandType::SKIP_TURN:
            return std::make_unique<SkipTurnCommand>();
        
        case CommandType::CAST_SPELL: {
            int spellIndex = parseSpellIndex(spellHand);
            if (spellIndex < 0) {
                return std::make_unique<InvalidCommand>("Invalid spell selection");
            }
            Position target = parsePositionInput();
            if (target.getX() < 0 || target.getY() < 0) {
                return std::make_unique<InvalidCommand>("Invalid target position");
            }
            return std::make_unique<CastSpellCommand>(spellIndex, target);
        }
        
        case CommandType::BUY_SPELL:
            return std::make_unique<BuySpellCommand>();
        
        case CommandType::SAVE_GAME:
            return std::make_unique<SaveGameCommand>();
        
        case CommandType::RESTART:
            return std::make_unique<RestartCommand>();
        
        case CommandType::QUIT:
            return std::make_unique<QuitCommand>();
        
        default:
            return std::make_unique<InvalidCommand>("Unknown command: " + trimmed);
    }
}
