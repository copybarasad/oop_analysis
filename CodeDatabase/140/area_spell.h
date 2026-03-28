#pragma once
#include "spell.h"

class AreaDamageSpell : public Spell {
private:
    int damage;
    int radius;
public:
    AreaDamageSpell(int dmg = 20, int rad = 5)
        : damage(dmg), radius(rad) {}

    void use(Field& field, int caster_x, int caster_y, int target_x, int target_y) override;
    Spell* clone() const override { return new AreaDamageSpell(*this); }
    string get_name() const override { return "Area"; }
    void upgrade() override {
        damage += 5;
        radius += 5;
    }

    int get_radius() const;
    void set_radius(int r);
};