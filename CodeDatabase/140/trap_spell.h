#pragma once
#include "spell.h"

class TrapSpell : public Spell {
private:
    int damage;
    int radius;
public:
    TrapSpell(int dmg = 20, int rad = 5)
        : damage(dmg), radius(rad) {}

    void use(Field& field, int caster_x, int caster_y, int target_x, int target_y) override;
    Spell* clone() const override { return new TrapSpell(*this); }
    string get_name() const override { return "Trap"; }
    void upgrade() override {
        damage += 10;
        radius++;
    }

    int get_damage() const;
    void set_damage(int d);
};