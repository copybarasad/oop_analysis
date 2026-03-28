#ifndef SPELLAREADAMAGE_H
#define SPELLAREADAMAGE_H

#include "Spell.h"

class SpellAreaDamage : public Spell {
private:
    int area_size;

public:
    SpellAreaDamage(int damage, int radius);
    void UseSpell(int target_x, int target_y, Map& map) override;
    bool CanCast(int caster_x, int caster_y, int target_x, int target_y, Map& map) override;

protected:
    void UpdateInfo() override {
        SetInfo("Area Damage - Damage: " + std::to_string(damage) + ", Radius: " + std::to_string(range) + ", Area: 2x2");
    }
};

#endif