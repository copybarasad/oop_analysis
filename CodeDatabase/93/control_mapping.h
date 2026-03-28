#ifndef CONTROL_MAPPING_H
#define CONTROL_MAPPING_H

#include <map>
#include <string>

enum class CommandId {
    MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT,
    ATTACK_UP, ATTACK_DOWN, ATTACK_LEFT, ATTACK_RIGHT,
    SWITCH_MODE,
    BUY_SPELL,
    USE_SPELL_1, USE_SPELL_2, USE_SPELL_3,
    SAVE_GAME,
    EXIT_GAME
};

struct ControlMapping {
    std::map<char, CommandId> keyToCommand;
    std::map<CommandId, char> commandToKey;

    static ControlMapping defaultMapping();
    bool isValid() const;
};

#endif