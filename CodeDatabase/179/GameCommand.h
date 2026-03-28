#ifndef GAMECOMMAND_H
#define GAMECOMMAND_H

#include "Position.h"  // Убедитесь, что эта строка есть

enum class CommandType {
    MOVE_UP,
    MOVE_DOWN,
    MOVE_LEFT,
    MOVE_RIGHT,
    SWITCH_MODE,
    CAST_SPELL,
    RANGED_ATTACK,  // Добавляем команду дальней атаки
    SAVE_GAME,
    LOAD_GAME,
    EXIT,
    SHOW_SPELLS
};

struct GameCommand {
    CommandType type;
    Position target; // Для заклинаний и дальней атаки
    size_t spellIndex; // Индекс заклинания
    
    GameCommand(CommandType cmdType) : type(cmdType), spellIndex(0) {}
    GameCommand(CommandType cmdType, size_t index) : type(cmdType), spellIndex(index) {}
    GameCommand(CommandType cmdType, const Position& pos) : type(cmdType), target(pos), spellIndex(0) {}
};

#endif
