#pragma once

#include "Spell.h"
#include "Constants.h"

class TrapSpell: public Spell {
public:
    TrapSpell(int r = 1, int v = 60, int dist = 7)
        : Spell(r, v, dist, SpellType::Trap) {}

    bool use(GameField& gameField, int x, int y, int current_dist, int& player_boost_level_) override;
    bool use(GameField& gameField, Tower* tower) override;

    TrapSpell* clone() const override {
        return new TrapSpell(*this);
    }
};