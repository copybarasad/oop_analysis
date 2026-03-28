#pragma once
#include "spell.h"

class LevelupSpell : public Spell {
public:
    void use(Field& field, int caster_x, int caster_y, int target_x, int target_y) override;
    Spell* clone() const override { return new LevelupSpell(*this); }
    string get_name() const override { return "Level"; }
};