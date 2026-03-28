#ifndef TRAPSPELL_H
#define TRAPSPELL_H

#include "Spell.h"

/**
 * @file TrapSpell.h
 * @brief Заклинание установки ловушки на поле
 */

class TrapSpell : public Spell {
private:
    int trapDamage;
    
public:
    TrapSpell(int baseDamage = 35);
    
    bool cast(Game* game) override;
    bool cast(Game* game, int targetX, int targetY) override;
    
    bool requiresTarget() const override {
        return true;
    }
    
    void increaseDamage(int extraDamage) { 
        trapDamage += extraDamage; 
    }
    
    int getDamage() const { 
        return trapDamage; 
    }
};

#endif // TRAPSPELL_H


