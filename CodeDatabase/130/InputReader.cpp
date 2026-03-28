#include "InputReader.h"
#include "InputUtils.h"
#include <iostream>

ConsoleInputReader::ConsoleInputReader(const InputConfig& config) : config_(config) {}

Command ConsoleInputReader::buildMoveCommand(CommandType type) const {
    Command cmd;
    cmd.type = type;
    if (type == CommandType::MOVE_UP) cmd.dy = -1;
    if (type == CommandType::MOVE_DOWN) cmd.dy = 1;
    if (type == CommandType::MOVE_LEFT) cmd.dx = -1;
    if (type == CommandType::MOVE_RIGHT) cmd.dx = 1;
    return cmd;
}

Command ConsoleInputReader::buildAttackCommand() {
    std::cout << "Attack direction (W/A/S/D): ";
    char d;
    if (!(std::cin >> d)) {
        std::cin.clear();
        std::string dummy; std::getline(std::cin, dummy);
        return {CommandType::INVALID, 0, 0, 0};
    }
    char nd = normalizeKey(d);
    if (nd == 'w') return {CommandType::ATTACK, 0, -1, d};
    if (nd == 's') return {CommandType::ATTACK, 0, 1, d};
    if (nd == 'a') return {CommandType::ATTACK, -1, 0, d};
    if (nd == 'd') return {CommandType::ATTACK, 1, 0, d};
    return {CommandType::INVALID, 0, 0, d};
}

Command ConsoleInputReader::readCommand() {
    std::cout << "Enter command: ";
    char ch;
    if (!(std::cin >> ch)) {
        std::cin.clear();
        std::string dummy; std::getline(std::cin, dummy);
        return {CommandType::INVALID, 0, 0, 0};
    }
    char normalized = normalizeKey(ch);
    CommandType type = config_.commandForKey(normalized);
    Command cmd;
    switch (type) {
        case CommandType::MOVE_UP:
        case CommandType::MOVE_DOWN:
        case CommandType::MOVE_LEFT:
        case CommandType::MOVE_RIGHT:
            cmd = buildMoveCommand(type);
            break;
        case CommandType::ATTACK:
            cmd = buildAttackCommand();
            break;
        case CommandType::CAST_SPELL:
        case CommandType::SAVE:
        case CommandType::SWITCH_MODE:
        case CommandType::QUIT:
            cmd.type = type;
            break;
        default:
            cmd.type = CommandType::INVALID;
            break;
    }
    cmd.raw = ch;
    return cmd;
}


