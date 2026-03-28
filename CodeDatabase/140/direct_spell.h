#pragma once
#include "spell.h"

class DirectDamageSpell : public Spell {
private:
    int damage;
    int radius;
public:
    DirectDamageSpell(int dmg = 50, int rad = 5)
        : damage(dmg), radius(rad) {}
        
    void use(Field& field, int caster_x, int caster_y, int target_x, int target_y) override;
    Spell* clone() const override {return new DirectDamageSpell(*this);}
    string get_name() const override {return "Direct";}
    void upgrade() override {
        damage += 10;
        radius++;
    }

    int get_radius() const;
    void set_radius(int r);
};