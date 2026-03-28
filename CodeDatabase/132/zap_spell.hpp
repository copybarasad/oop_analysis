#pragma once

#include "effect.hpp"
#include "damage_spell.hpp"

class ZapSpell : public DamageSpell {
private:
    int default_damage;
public:
    ZapSpell();
    ZapSpell(int default_damage);

    int apply_enhancement(int current_lvl) override;
    bool use(FieldHandler& field_handler, const Position& from, const Position& target) override;
};