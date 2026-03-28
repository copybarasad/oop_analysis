#ifndef DIRECTDAMAGESPELL_H
#define DIRECTDAMAGESPELL_H

#include "Spell.h"

/**
 * @file DirectDamageSpell.h
 * @brief Заклинание прямого урона по одной цели
 */

class DirectDamageSpell : public Spell {
private:
    int damage;
    int baseRadius;
    
public:
    DirectDamageSpell(int baseDamage = 40, int radius = 3);
    
    bool cast(Game* game) override;
    
    bool requiresTarget() const override {
        return false;
    }
    
    void increaseRadius(int extraRadius) { 
        baseRadius += extraRadius; 
    }
    
    int getRadius() const { 
        return baseRadius; 
    }
};

#endif // DIRECTDAMAGESPELL_H


