#pragma once

#include "Spell.h"
#include "Constants.h"

class InstantDamageSpell: public Spell {
public:
    InstantDamageSpell(int r = 1, int v = 40, int dist = 4)
        : Spell(r, v, dist, SpellType::InstantDamage) 
        {}

    bool use(GameField& gameField, int x, int y, int current_dist, int& player_boost_level_) override;
    bool use(GameField& gameField, Tower* tower) override;

    InstantDamageSpell* clone() const override {
        return new InstantDamageSpell(*this);
    }
};