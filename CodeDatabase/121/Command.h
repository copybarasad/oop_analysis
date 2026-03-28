#ifndef COMMAND_H
#define COMMAND_H

enum class Command {
    MOVE_UP,
    MOVE_DOWN, 
    MOVE_LEFT,
    MOVE_RIGHT,
    ATTACK,
    SWITCH_WEAPON,
    USE_SPELL_1,
    USE_SPELL_2,
    USE_SPELL_3,
    SHOW_SPELLS,
    SHOP,           
    SAVE_GAME,      
    LOAD_GAME,      
    EXIT_GAME,
    UNKNOWN
};

#endif