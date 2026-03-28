#ifndef SPELLDIRECTDAMAGE_H
#define SPELLDIRECTDAMAGE_H

#include "Spell.h"

class SpellDirectDamage : public Spell {
public:
    SpellDirectDamage(int damage, int radius);
    void UseSpell(int target_x, int target_y, Map& map) override;
    bool CanCast(int caster_x, int caster_y, int target_x, int target_y, Map& map) override;

protected:
    void UpdateInfo() override {
        SetInfo("Direct Damage - Damage: " + std::to_string(damage) + ", Radius: " + std::to_string(range));
    }
};

#endif