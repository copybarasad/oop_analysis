#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "spell.h"

class AreaDamageSpell : public Spell {
private:
    int damage;
    int range;

public:
    AreaDamageSpell(int spellDamage, int spellRange);
    
    bool cast(Game& game, int targetX, int targetY) override;
    bool isValidTarget(const Game& game, int targetX, int targetY) const override;
    
    std::string getName() const override;
    std::string getDescription() const override;
    int getManaCost() const override;
    int getRange() const override;
    int getDamage() const override;
    void increaseDamage(int amount) override;
};

#endif