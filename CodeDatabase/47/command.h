#ifndef COMMAND_H
#define COMMAND_H

enum class CommandType {
    MoveUp, MoveDown, MoveLeft, MoveRight,
    Attack,
    SwitchCombatMode,
    CastSpell1, CastSpell2, CastSpell3,
    SaveGame,
    LoadGame,
    Pause,
    Quit,
    None
};

struct Command {
    CommandType type = CommandType::None;
    int paramX = 0;
    int paramY = 0;
};

#endif
