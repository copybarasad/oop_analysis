#ifndef PLAYER_COMMAND_H
#define PLAYER_COMMAND_H

#include "Location.h"
#include "MoveDirection.h"
#include <string>

enum class CommandType {
    MOVE,
    SWITCH_COMBAT,
    RANGED_ATTACK,
    CAST_SPELL,
    OPEN_SHOP,
    SHOW_INFO,
    SAVE_GAME,
    LOAD_GAME,
    MAIN_MENU,
    EXIT_GAME,
    INVALID
};

struct PlayerCommand {
    CommandType type;
    MoveDirection direction;
    int spellIndex;
    Location target;
    std::string filename;
    
    PlayerCommand() : type(CommandType::INVALID), spellIndex(-1) {}
};

#endif // PLAYER_COMMAND_H