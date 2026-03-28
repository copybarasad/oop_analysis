#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "Spell.h"

class Game;

class AreaDamageSpell : public Spell {
private:
    int enhancedAreaSize;

public:
    AreaDamageSpell(int radius = 4, int damage = 20, int areaSize = 2);
    
    SpellResult use(Game& game, int targetX, int targetY) override;
    std::string getName() const override;
    std::string getDescription() const override;
    void applyEnhancement(int radiusBonus, int damageBonus, int areaBonus, int summonBonus) override;
    std::unique_ptr<Spell> clone() const override;
};

#endif // AREADAMAGESPELL_H

