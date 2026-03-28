#include "KeyboardInputHandler.h"
#include "../utils /ConfigLoader.h"
#include <iostream>
#include <cctype>

#include "../commands/BuySpellCommand.h"
#include "../commands/MoveCommand.h"
#include "../commands/QuitCommand.h"
#include "../commands/SaveCommand.h"
#include "../commands/ShootCommand.h"
#include "../commands/SwitchFightModeCommand.h"
#include "../commands/UnknownCommand.h"
#include "../commands/UseSpellCommand.h"

KeyboardInputHandler::KeyboardInputHandler(const std::string& configFile)
    : configFile(configFile) {

    if (!configFile.empty()) {
        loadConfig(configFile);
    } else {
        setDefaultBindings();
    }
}

void KeyboardInputHandler::loadConfig(const std::string& filename) {
    keyBindings = ConfigLoader::loadFromFile(filename);

    if (keyBindings.empty() || !validateConfig()) {
        std::cout << "Warning: Invalid or empty configuration. Using default bindings." << std::endl;
        setDefaultBindings();
    }
}

void KeyboardInputHandler::setDefaultBindings() {
    keyBindings = {
        {'w', CommandType::MOVE_UP},
        {'s', CommandType::MOVE_DOWN},
        {'a', CommandType::MOVE_LEFT},
        {'d', CommandType::MOVE_RIGHT},
        {'c', CommandType::SWITCH_FIGHT_MODE},
        {'f', CommandType::SHOOT},
        {'b', CommandType::BUY_SPELL},
        {'x', CommandType::USE_SPELL},
        {'q', CommandType::QUIT_GAME}
    };
}

bool KeyboardInputHandler::validateConfig() const {
    bool hasUp = false, hasDown = false, hasLeft = false, hasRight = false, hasQuit = false;

    for (const auto& [key, cmd] : keyBindings) {
        switch(cmd) {
            case CommandType::MOVE_UP: hasUp = true; break;
            case CommandType::MOVE_DOWN: hasDown = true; break;
            case CommandType::MOVE_LEFT: hasLeft = true; break;
            case CommandType::MOVE_RIGHT: hasRight = true; break;
            case CommandType::QUIT_GAME: hasQuit = true; break;
            default: break;
        }
    }

    return hasUp && hasDown && hasLeft && hasRight && hasQuit;
}

std::unique_ptr<Command> KeyboardInputHandler::getNextCommand() {
    std::cout << "Commands: ";
    for (const auto& [key, cmd] : keyBindings) {
        switch (cmd) {
            case CommandType::MOVE_UP: std::cout << "[" << key << "]up "; break;
            case CommandType::MOVE_DOWN: std::cout << "[" << key << "]down "; break;
            case CommandType::MOVE_LEFT: std::cout << "[" << key << "]left "; break;
            case CommandType::MOVE_RIGHT: std::cout << "[" << key << "]right "; break;
            case CommandType::SWITCH_FIGHT_MODE: std::cout << "[" << key << "]switch "; break;
            case CommandType::SHOOT: std::cout << "[" << key << "]shoot "; break;
            case CommandType::BUY_SPELL: std::cout << "[" << key << "]buy "; break;
            case CommandType::USE_SPELL: std::cout << "[" << key << "]spells "; break;
            case CommandType::QUIT_GAME: std::cout << "[" << key << "]quit "; break;
            default: break;
        }
    }
    std::cout << "\nEnter command: ";

    char op;
    std::cin >> op;
    clearLine();

    op = std::tolower(op);

    auto it = keyBindings.find(op);
    if (it != keyBindings.end()) {
        return createCommand(it->second);
    }

    return std::make_unique<UnknownCommand>(op);
}

Direction KeyboardInputHandler::getShootDirection() const {
    do {
        std::cout << "Shoot direction [W]up, [A]left, [S]down, [D]right: ";
        char dir;
        std::cin >> dir;
        clearLine();

        switch (std::tolower(dir)) {
            case 'w': return Direction::Up;
            case 'a': return Direction::Left;
            case 's': return Direction::Down;
            case 'd': return Direction::Right;
            default:
                std::cout << "Invalid direction! Try again." << std::endl;
                break;
        }
    } while (true);
}

std::unique_ptr<Command> KeyboardInputHandler::getSaveMenuCommand() {
    do {
        std::cout << "Do you want to save this game? [y/n]: ";
        char ch;
        std::cin >> ch;
        clearLine();

        switch (std::tolower(ch)) {
            case 'y': return std::make_unique<SaveCommand>(true);
            case 'n': return std::make_unique<SaveCommand>(false);
            default:
                std::cout << "Invalid choice! Enter 'y' or 'n'." << std::endl;
                break;
        }
    } while (true);
}

std::unique_ptr<Command> KeyboardInputHandler::createCommand(CommandType type) const {
    switch (type) {
        case CommandType::MOVE_UP:
            return std::make_unique<MoveCommand>(Direction::Up);
        case CommandType::MOVE_DOWN:
            return std::make_unique<MoveCommand>(Direction::Down);
        case CommandType::MOVE_LEFT:
            return std::make_unique<MoveCommand>(Direction::Left);
        case CommandType::MOVE_RIGHT:
            return std::make_unique<MoveCommand>(Direction::Right);
        case CommandType::SWITCH_FIGHT_MODE:
            return std::make_unique<SwitchFightModeCommand>();
        case CommandType::SHOOT: {
            Direction dir = getShootDirection();
            return std::make_unique<ShootCommand>(dir);
        }
        case CommandType::BUY_SPELL:
            return std::make_unique<BuySpellCommand>();
        case CommandType::USE_SPELL:
            return std::make_unique<UseSpellCommand>();
        case CommandType::QUIT_GAME:
            return std::make_unique<QuitCommand>();
        default:
            return nullptr;
    }
}

void KeyboardInputHandler::clearLine() const {
    std::cout << "\033[A" << "\r" << "\033[K";
}
