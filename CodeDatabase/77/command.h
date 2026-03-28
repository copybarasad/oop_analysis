#pragma once

enum class CommandType {
    None, Quit, Save, Load,
    Move, Attack, SwitchMode, CastSpell, BuySpell
};

struct GameCommand {
    CommandType type;
    int x = 0;      
    int y = 0;      
    int param = 0;  
};