#pragma once

#include "Spell.h"
#include "Constants.h"

class AllySpell: public Spell {
public:
    AllySpell(int r = 1, int v = 1, int dist = 5): Spell(r, v, dist, SpellType::Ally) {}

    bool use(GameField& gameField, int x, int y, int current_dist, int& player_boost_level_) override;
    bool use(GameField& gameField, Tower* tower) override;

    AllySpell* clone() const override {
        return new AllySpell(*this);
    }
};