#pragma once

#include "Entity.hpp"
#include "Hand.hpp"

// Player with two modes: melee or ranged. Stores mana and a hand of spells.
class Player : public Entity {
public:
    enum class Mode { Melee, Ranged };

private:
    Mode mode_{Mode::Melee};
    int melee_bonus_{2};
    int ranged_bonus_{1};

    int mana_{0};
    SpellHand hand_;

public:
    Player() : hand_(HandConfig{3}) {}

    Player(int hp, int atk, Pos p, int meleeBonus = 2, int rangedBonus = 1, HandConfig handCfg = HandConfig{3})
        : Entity(hp, atk, p),
          mode_(Mode::Melee),
          melee_bonus_(meleeBonus),
          ranged_bonus_(rangedBonus),
          hand_(handCfg) {}

    void toggle_mode() { mode_ = (mode_ == Mode::Melee) ? Mode::Ranged : Mode::Melee; }
    Mode mode() const { return mode_; }

    // Total damage depends on current mode.
    int damage() const {
        return atk_ + ((mode_ == Mode::Melee) ? melee_bonus_ : ranged_bonus_);
    }
    int melee_bonus() const { return melee_bonus_; }
    int ranged_bonus() const { return ranged_bonus_; }

    int mana() const { return mana_; }
    void set_mana(int v) { mana_ = (v < 0 ? 0 : v); }
    void add_mana(int v) { if (v > 0) mana_ += v; }
    bool spend_mana(int v) {
        if (v <= 0) return true;
        if (mana_ < v) return false;
        mana_ -= v;
        return true;
    }

    SpellHand& hand() { return hand_; }
    const SpellHand& hand() const { return hand_; }
};
