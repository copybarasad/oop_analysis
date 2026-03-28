#ifndef GAME_COMMAND_H
#define GAME_COMMAND_H

#include <utility>
#include <string>

struct GameCommand {
    enum class Type {
        MOVE_UP,
        MOVE_DOWN,
        MOVE_LEFT,
        MOVE_RIGHT,
        SWITCH_MODE,
        RANGED_ATTACK,
        CAST_SPELL,
        SAVE_GAME,
        LOAD_GAME,
        QUIT,
        INVALID
    };

    Type type;
    int spellIndex;
    std::pair<int, int> target;
    std::string filename;

    GameCommand() 
        : type(Type::INVALID), spellIndex(-1), target(0, 0), filename("") {}
    
    GameCommand(Type t) 
        : type(t), spellIndex(-1), target(0, 0), filename("") {}
};

#endif