#pragma once
#include <string>

enum class CommandType {
    None,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Attack,
    CastSpell,
    SaveGame,
    LoadGame,
    ExitToMenu,      
    SwitchAttackType,
    NewGame,          
    QuitGame          
};
struct Command {
    CommandType type;
    char rawKey; 
};
