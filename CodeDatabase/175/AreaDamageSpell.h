#pragma once
#include "Spell.h"

class AreaDamageSpell : public Spell {
private:
    int baseDamage_;
    int areaSize_;
    int sizeMultiplier_;

public:
    AreaDamageSpell(int damage = 10, int size = 2, int mult = 1);
    std::string getName() const override;
    bool apply(Field& field, const Player& player, int targetX, int targetY) override;
    void setSizeMultiplier(int mult) { sizeMultiplier_ = mult; }
    int getSizeMultiplier() const { return sizeMultiplier_; }
    bool needsTarget() const override { return false; }
};