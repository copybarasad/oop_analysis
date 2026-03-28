#pragma once
#include "spell.h"

class CreateSpell : public Spell {
private:
    int ally_hp;
    int ally_damage;
    int radius;
    int ally_count = 1;

public:
    CreateSpell(int hp = 30, int dmg = 20, int rad = 1);

    void use(Field& field, int caster_x, int caster_y, int target_x, int target_y) override;
    Spell* clone() const override { return new CreateSpell(*this); }
    std::string get_name() const override { return "Create"; }
    void upgrade() override {
        ally_hp += 20;
        ally_damage += 10;
    }

    void set_ally_count(int count);
    int get_ally_count() const;
};