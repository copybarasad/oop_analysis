#ifndef SPELLTRAP_H
#define SPELLTRAP_H

#include "Spell.h"

class SpellTrap : public Spell {
public:
    SpellTrap(int damage, int radius);
    void UseSpell(int target_x, int target_y, Map& map) override;
    bool CanCast(int caster_x, int caster_y, int target_x, int target_y, Map& map) override;

protected:
    void UpdateInfo() override {
        SetInfo("Trap Spell - Damage: " + std::to_string(damage) + ", Radius: " + std::to_string(range));
    }
};

#endif