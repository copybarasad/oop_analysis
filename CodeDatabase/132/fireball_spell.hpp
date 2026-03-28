#pragma once

#include "bullet.hpp"
#include "damage_spell.hpp"

class FireballSpell : public DamageSpell {
public:
    FireballSpell();

    int apply_enhancement(int current_lvl) override;
    bool use(FieldHandler& field_handler, const Position& from, const Position& target) override;
};