#pragma once

#include "Spell.h"
#include "Constants.h"

class BoostSpell: public Spell {
public:
    BoostSpell(int x = 1, int v = 1, int dist = 3): Spell(x, v, dist, SpellType::Boost) {}
    bool use(GameField& gameField, int x, int y, int current_dist, int& player_boost_level_) override;
    bool use(GameField& gameField, Tower* tower) override;

    BoostSpell* clone() const override {
        return new BoostSpell(*this);
    }
};