#pragma once

#include <string>
#include "direction.h"

enum class CommandType {
    None,
    Move,
    ToggleAttackMode,
    CastSpell,
    RangedAttack,
    BuySpell,
    SaveGame,
    LoadGame,
    ExitGame
};

class Command {
public:
    CommandType type = CommandType::None;
    Direction direction = Direction::UP;
    int spellIndex = -1;
    int enemyIndex = -1;
    std::string filename;
    std::string spellName;
};
