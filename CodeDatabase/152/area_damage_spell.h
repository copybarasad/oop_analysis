#ifndef GAME_AREA_DAMAGE_SPELL_H_
#define GAME_AREA_DAMAGE_SPELL_H_
#include "spell.h"
#include <iostream>
class AreaDamageSpell : public Spell {
public:
    explicit AreaDamageSpell(int damage, int radius = 2)
        : damage_(damage), radius_(radius) {
    }
    void use(Game& game, GameField& field, Player& player, int target_row, int target_col) override;
    const char* name() const override { return "Урон по области"; }
    int getType() const override { return 1; }
    int getDamage() const override { return damage_; }
    int getRadius() const override { return radius_; }
private:
    int damage_;
    int radius_;
};
#endif