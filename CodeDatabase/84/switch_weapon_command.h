#ifndef SWITCH_WEAPON_COMMAND_H
#define SWITCH_WEAPON_COMMAND_H

#include "command.h"

class SwitchWeaponCommand : public Command {
    bool melee_;
public:
    SwitchWeaponCommand(bool to_melee) : melee_(to_melee) {}
    bool execute(Game& game) override {
        game.switch_weapon(melee_);
        return true;
    }
};

#endif