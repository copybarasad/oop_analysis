#ifndef BOMB_H
#define BOMB_H

#include "Spell.h"

class BOMB : public Spell {
private:
    int damage;
    int radius;

public:
    BOMB(int id, const std::string& name, int cost, int damage, int radius);
    bool isAreaSpell() const override { return true; }
    void apply(Character& caster, Character& target) const override;
    void applyArea(Character& caster, int centerX, int centerY, Game& game) override;
};

#endif