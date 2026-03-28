#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "spell.h"

class DirectDamageSpell : public Spell {
private:
    int damage;
    int range;
    
public:
    DirectDamageSpell(int dmg, int rng);
    std::string getName() const override;
    std::string getDescription() const override;
    bool canCast(int playerX, int playerY, int targetX, int targetY) const override;
    void cast(Game& game, int targetX, int targetY) override;
    int getManaCost() const override;
};

#endif