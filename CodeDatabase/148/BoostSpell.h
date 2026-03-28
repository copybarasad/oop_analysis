#ifndef BOOSTSPELL_H
#define BOOSTSPELL_H

#include "Spell.h"

/**
 * @file BoostSpell.h
 * @brief Заклинание улучшения следующего заклинания
 */

class BoostSpell : public Spell {
public:
    BoostSpell();
    
    bool cast(Game* game) override;
    
    bool requiresTarget() const override {
        return false;
    }
    
    bool isBoostSpell() const override { 
        return true; 
    }
};

#endif // BOOSTSPELL_H


