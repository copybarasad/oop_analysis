#pragma once
#include <string>

struct GameCommand {
    enum class Type {
        MOVE_UP,
        MOVE_DOWN, 
        MOVE_LEFT,
        MOVE_RIGHT,
        SHOW_STATS,
        SHOW_INSTRUCTIONS,
        SHOW_SPELLS,
        BUY_SPELLS,
        CAST_SPELL,
        CHANGE_COMBAT_RANGE,
        SAVE_GAME,
        LOAD_GAME,
        EXIT,
        INVALID
    };
    
    Type type;
    int spellIndex;
    int combatRange;
    std::string data;
    
    GameCommand(Type t = Type::INVALID, const std::string& d = "", int si = -1, int cr = 0) 
        : type(t), spellIndex(si), combatRange(cr), data(d) {}
};