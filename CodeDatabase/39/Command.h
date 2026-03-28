#pragma once

enum class CommandType {
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    SwitchMode,
    RangedAttack,
    CastSpell,
    Save,
    Load,
    Quit,
    None
};

struct Command {
    CommandType type;
};
