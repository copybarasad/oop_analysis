#ifndef COMMAND_HPP
#define COMMAND_HPP

enum class Command {
    Up, Down, Left, Right,
    Attack, SwitchAttackType,
    ShowHand, BuySpell, UseSpell,
    Exit, Save, Load, Help
};

#endif