#ifndef AREADAMAGESPELL_H
#define AREADAMAGESPELL_H

#include "Spell.h"

/**
 * @file AreaDamageSpell.h
 * @brief Заклинание урона по площади
 */

class AreaDamageSpell : public Spell {
private:
    int damage;
    int areaSize;
    
public:
    AreaDamageSpell(int baseDamage = 25, int size = 2);
    
    bool cast(Game* game) override;
    bool cast(Game* game, int targetX, int targetY) override;
    
    bool requiresTarget() const override {
        return true;
    }
};

#endif // AREADAMAGESPELL_H


