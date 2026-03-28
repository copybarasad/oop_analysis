#ifndef COMMAND_H
#define COMMAND_H

enum class CommandType {
    None,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    Attack,      
    ShowStats,
    UseSpell,    
    Save,
    Load,
    Quit,
    PrintField,
    SwitchAttackType,
    SearchEnemies
};

struct Command {
    CommandType type;
    int param; // для индекса заклинания по сути 
    Command(CommandType t = CommandType::None, int p = -1) : type(t), param(p) {}
};

#endif
