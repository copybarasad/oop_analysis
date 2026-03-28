#ifndef UPDATESPELL_H
#define UPDATESPELL_H

#include "SpellBase.h"
#include "IUpgradableSpell.h"

class HandSpell;

class UpdateSpell : public SpellBase {
private:
    int maxLevel;
    
public:
    UpdateSpell(int curLevel, int maxLevel);
    
    bool use(int x, int y, GameField& field, Player& player) override;
    
    
    void resetUpdate(HandSpell& handSpell);
    int getMaxLevel() const;
    void setMaxLevel(int newMaxLevel);
};

#endif