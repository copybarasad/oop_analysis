#pragma once

enum class CommandType {
    None,
    Move,
    SwitchMode,
    CastSpell,
    SaveGame,
    QuitToMenu,
    Invalid
};

struct Command {
    CommandType type{CommandType::None};
    int dx{0};
    int dy{0};
    int spellIndex{-1};
    int targetX{0};
    int targetY{0};
};

