#pragma once
#include "Spell.h"

class TrapSpell : public Spell {
private:
    int baseDamage_;
    int damageMultiplier_;

public:
    TrapSpell(int damage = 15, int mult = 1);
    std::string getName() const override;
    bool apply(Field& field, const Player& player, int targetX, int targetY) override;
    void setDamageMultiplier(int mult) { damageMultiplier_ = mult; }
    int getDamageMultiplier() const { return damageMultiplier_; }
};