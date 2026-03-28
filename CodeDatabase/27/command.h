#pragma once

enum class CommandType {
    START_NEW_GAME,
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SWITCH_WEAPON,
    CAST_SPELL, 
    OPEN_SHOP,
    SAVE_GAME,
    LOAD_GAME,
    EXIT_GAME,
    LEVEL_UP_HP,
    LEVEL_UP_DAMAGE,
    LEVEL_UP_SKIP,
    INVALID 
};

struct Command {
    CommandType type;
    int target_x = -1;
    int target_y = -1;
    int spell_index = -1;
    bool buy_spell = false;
};
