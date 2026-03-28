#pragma once

#include "Position.hpp"

// Nest periodically spawns enemies until destroyed.
class EnemyNest {
    Pos pos_;
    int period_{5};
    int counter_{5};
    int hp_{4};

public:
    EnemyNest(Pos p, int period, int hp = 4)
        : pos_(p), period_(period), counter_(period), hp_(hp) {}

    Pos pos() const { return pos_; }
    int period() const { return period_; }
    int counter() const { return counter_; }
    int hp() const { return hp_; }
    bool alive() const { return hp_ > 0; }

    // Returns true when it is time to spawn.
    bool tick() {
        if (!alive()) return false;
        if (counter_ > 0) --counter_;
        return counter_ == 0;
    }

    void reset() { counter_ = period_; }
    void set_counter(int value) { counter_ = value; }
    void set_hp(int value) { hp_ = (value < 0 ? 0 : value); }

    bool take_damage(int dmg) {
        if (!alive() || dmg <= 0) return false;
        hp_ -= dmg;
        if (hp_ < 0) hp_ = 0;
        return true;
    }
};
