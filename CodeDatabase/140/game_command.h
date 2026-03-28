#pragma once

#include <variant>
#include <string>

enum class CommandType {
    Quit,
    Help,
    Move,
    SwitchMode,
    Attack,
    UseSpell,
    Save,
    Load,
    None
};

struct GameCommand {
    CommandType type = CommandType::None;

    std::string direction; 
    int x = 0, y = 0;   
    std::size_t spellIndex = 0; 
};
