#include "control_mapping.h"
#include <vector>
#include <algorithm>

ControlMapping ControlMapping::defaultMapping() {
    ControlMapping m;
    m.keyToCommand = {
        {'W', CommandId::MOVE_UP},   {'w', CommandId::MOVE_UP},
        {'S', CommandId::MOVE_DOWN}, {'s', CommandId::MOVE_DOWN},
        {'A', CommandId::MOVE_LEFT}, {'a', CommandId::MOVE_LEFT},
        {'D', CommandId::MOVE_RIGHT},{'d', CommandId::MOVE_RIGHT},
        {'I', CommandId::ATTACK_UP},   {'i', CommandId::ATTACK_UP},
        {'K', CommandId::ATTACK_DOWN}, {'k', CommandId::ATTACK_DOWN},
        {'J', CommandId::ATTACK_LEFT}, {'j', CommandId::ATTACK_LEFT},
        {'L', CommandId::ATTACK_RIGHT},{'l', CommandId::ATTACK_RIGHT},
        {'M', CommandId::SWITCH_MODE}, {'m', CommandId::SWITCH_MODE},
        {'B', CommandId::BUY_SPELL},   {'b', CommandId::BUY_SPELL},
        {'1', CommandId::USE_SPELL_1},
        {'2', CommandId::USE_SPELL_2},
        {'3', CommandId::USE_SPELL_3},
        {'Z', CommandId::SAVE_GAME},   {'z', CommandId::SAVE_GAME},
        {'Q', CommandId::EXIT_GAME},   {'q', CommandId::EXIT_GAME}
    };

    for (const auto& pair : m.keyToCommand) {
        m.commandToKey[pair.second] = pair.first;
    }
    return m;
}

bool ControlMapping::isValid() const {
    if (keyToCommand.empty() || commandToKey.empty()) 
        return false;

    std::vector<CommandId> allCmds = {
        CommandId::MOVE_UP, CommandId::MOVE_DOWN, CommandId::MOVE_LEFT, CommandId::MOVE_RIGHT,
        CommandId::ATTACK_UP, CommandId::ATTACK_DOWN, CommandId::ATTACK_LEFT, CommandId::ATTACK_RIGHT,
        CommandId::SWITCH_MODE, CommandId::BUY_SPELL,
        CommandId::USE_SPELL_1, CommandId::USE_SPELL_2, CommandId::USE_SPELL_3,
        CommandId::SAVE_GAME, CommandId::EXIT_GAME
    };

    for (CommandId cmd : allCmds) {
        if (commandToKey.find(cmd) == commandToKey.end()) 
            return false;
    }

    std::map<char, int> keyCount;
    for (const auto& pair : keyToCommand) {
        keyCount[pair.first]++;
    }
    for (const auto& pair : keyCount) {
        if (pair.second > 1) 
            return false;
    }

    std::map<CommandId, int> cmdCount;
    for (const auto& pair : keyToCommand) {
        cmdCount[pair.second]++;
    }
    for (const auto& pair : cmdCount) {
        if (pair.second > 1) 
            return false;
    }

    return true;
}