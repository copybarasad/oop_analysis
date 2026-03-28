#pragma once

enum class CommandType {
    None,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    AttackEnemy,
    AttackTower,    
    SwitchMode,
    UseSpell,
    Save,
    Load,
    Help,
    Quit
};

struct GameCommand {
    CommandType type = CommandType::None;
    int argument = -1; 
};
