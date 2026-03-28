#pragma once
#include "Spell.h"

class DirectDamageSpell : public Spell {
private:
    int baseDamage_;
    int radius_;
    int damageMultiplier_;

public:
    DirectDamageSpell(int damage = 20, int radius = 3, int mult = 1);
    std::string getName() const override;
    bool apply(Field& field, const Player& player, int targetX, int targetY) override;
    void setDamageMultiplier(int mult) { damageMultiplier_ = mult; }
    int getDamageMultiplier() const { return damageMultiplier_; }
};