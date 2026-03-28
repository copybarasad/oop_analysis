#pragma once

enum class CommandType {
    None,
    MoveUp,
    MoveDown,
    MoveLeft,
    MoveRight,
    SwitchWeapon,
    CastSpell,
    OpenShop,
    SaveGame,
    Quit
};

struct Command {
    CommandType type;
};
