#pragma once

#include "Spell.h"
#include "Constants.h"

class SplashDamageSpell : public Spell {
public:
    SplashDamageSpell(int radius = 2, int value = 10, int dist = 5)
        : Spell(radius, value, dist, SpellType::SplashDamage) {}

    bool use(GameField& gameField, int x, int y, int current_dist, int& player_boost_level_) override;
    bool use(GameField& gameField, Tower* tower) override;

    SplashDamageSpell* clone() const override {
        return new SplashDamageSpell(*this);
    }
};