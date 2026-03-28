#include "ConsoleInputHandler.h"
#include "ShopSystem.h"
#include "GameSaveSystem.h"
#include "Command.h"
#include "MoveCommand.h"
#include "CastSpellCommand.h"
#include "ShopCommand.h"
#include "SaveCommand.h"
#include "QuitCommand.h"
#include "CommandType.h"
#include <iostream>
#include <memory>
#include <cctype>

ConsoleInputHandler::ConsoleInputHandler()
    : shopSystem(nullptr), saveSystem(nullptr) {
}

CommandType ConsoleInputHandler::getCommandType() {
    char input = getRawInput();

    switch (std::tolower(static_cast<unsigned char>(input))) {
    case 'w': return CommandType::MOVE_UP;
    case 's': return CommandType::MOVE_DOWN;
    case 'a': return CommandType::MOVE_LEFT;
    case 'd': return CommandType::MOVE_RIGHT;
    case 'c': return CommandType::CAST_SPELL;
    case 'm': return CommandType::OPEN_SHOP;
    case 'p': return CommandType::SAVE_GAME;
    case 'q': return CommandType::QUIT_GAME;
    default:  return CommandType::INVALID;
    }
}

std::unique_ptr<Command> ConsoleInputHandler::createCommand(CommandType type) {
    switch (type) {
    case CommandType::MOVE_UP:
        return std::make_unique<MoveCommand>(0, -1);
    case CommandType::MOVE_DOWN:
        return std::make_unique<MoveCommand>(0, 1);
    case CommandType::MOVE_LEFT:
        return std::make_unique<MoveCommand>(-1, 0);
    case CommandType::MOVE_RIGHT:
        return std::make_unique<MoveCommand>(1, 0);
    case CommandType::CAST_SPELL:
        return std::make_unique<CastSpellCommand>(*this);
    case CommandType::OPEN_SHOP:
        if (shopSystem)
            return std::make_unique<ShopCommand>(*shopSystem, *this);
        break;
    case CommandType::SAVE_GAME:
        if (saveSystem)
            return std::make_unique<SaveCommand>(*saveSystem);
        break;
    case CommandType::QUIT_GAME:
        return std::make_unique<QuitCommand>();
    case CommandType::INVALID:
        break;
    }
    return nullptr;
}

char ConsoleInputHandler::getRawInput() {
    char input;
    std::cout << "Enter command (WASD=move, C=spells, M=shop, P=save, Q=quit): ";

    if (!(std::cin >> input)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Input error!" << std::endl;
        return 0;
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return input;
}

std::pair<int, int> ConsoleInputHandler::getSpellTarget() {
    int targetX, targetY;
    std::cout << "Enter target coordinates (x y): ";
    std::cin >> targetX >> targetY;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return { targetX, targetY };
}

int ConsoleInputHandler::getSpellChoice(int maxSpells) {
    int choice;
    std::cout << "Choose spell (1-" << maxSpells << "): ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

int ConsoleInputHandler::getShopChoice(int maxSpells) {
    int choice;
    std::cout << "Choose spell to buy (1-" << maxSpells << ") or 0 to cancel: ";
    std::cin >> choice;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}

void ConsoleInputHandler::setShopSystem(ShopSystem& shopSystem) {
    this->shopSystem = &shopSystem;
}

void ConsoleInputHandler::setSaveSystem(GameSaveSystem& saveSystem) {
    this->saveSystem = &saveSystem;
}