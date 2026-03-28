#ifndef SUMMONSPELL_H
#define SUMMONSPELL_H

#include "Spell.h"

/**
 * @file SummonSpell.h
 * @brief Заклинание призыва союзника
 */

class SummonSpell : public Spell {
private:
    int summonCount;
    
    bool findFreeAdjacentCell(Game* game, int centerX, int centerY, int& offsetX, int& offsetY);
    
public:
    SummonSpell(int count = 1);
    
    bool cast(Game* game) override;
    
    bool requiresTarget() const override {
        return false;
    }
    
    void increaseSummonCount(int extraCount) { 
        summonCount += extraCount; 
    }
    
    int getSummonCount() const { 
        return summonCount; 
    }
};

#endif // SUMMONSPELL_H


