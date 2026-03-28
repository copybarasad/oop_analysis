#ifndef GAME_DIRECT_DAMAGE_SPELL_H_
#define GAME_DIRECT_DAMAGE_SPELL_H_
#include "spell.h"
#include <cmath>
#include <iostream>
class DirectDamageSpell : public Spell {
public:
    explicit DirectDamageSpell(int damage, int radius = 1)
        : damage_(damage), radius_(radius) {
    }
    void use(Game& game, GameField& field, Player& player, int target_row, int target_col) override;
    const char* name() const override { return "Прямой урон"; }
    int getType() const override { return 0; }
    int getDamage() const override { return damage_; }
    int getRadius() const override { return radius_; }
private:
    int damage_;
    int radius_;
};
#endif