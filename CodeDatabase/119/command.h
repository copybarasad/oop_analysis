#pragma once

enum class CommandType {
    None, Quit, Save, Load, Start,
    Move, Attack, SwitchMode, CastSpell, BuySpell,
    UpgradeHp, UpgradeHand, UpgradeSpell, BackMenu
};

struct GameCommand {
    CommandType type;
    int x = 0;      
    int y = 0;      
    int param = 0;  
};