#pragma once

#include "Spell.h"
#include "Constants.h"

class HealthSpell: public Spell {
public:
    HealthSpell(int r = 2, int v = 60, int dist = 4)
    : Spell(1, 60, dist, SpellType::Health) {}

    bool use(GameField& gameField, int x, int y, int current_dist, int& player_boost_level_) override;
    bool use(GameField& gameField, Tower* tower) override;

    HealthSpell* clone() const override {
        return new HealthSpell(*this);
    }
};