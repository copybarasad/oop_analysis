#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "Spell.h"

class Game;

class DirectDamageSpell : public Spell {
private:
    int enhancedRadius;

public:
    DirectDamageSpell(int radius = 3, int damage = 25);
    
    SpellResult use(Game& game, int targetX, int targetY) override;
    std::string getName() const override;
    std::string getDescription() const override;
    void applyEnhancement(int radiusBonus, int damageBonus, int areaBonus, int summonBonus) override;
    std::unique_ptr<Spell> clone() const override;
};

#endif // DIRECTDAMAGESPELL_H

