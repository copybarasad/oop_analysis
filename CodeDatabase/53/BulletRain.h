#ifndef BULLETRAIN_H
#define BULLETRAIN_H

#include "Gamefield.h"
#include "SpellBase.h"
#include "IUpgradableSpell.h"
#include "IAreaSpell.h"

class BulletRain : public SpellBase, public IUpgradableSpell, public IAreaSpell {
private:
    int areaOfEffect;
    
public:
    BulletRain(int spell_level);
    
    bool use(int x, int y, GameField& field, Player& player) override;
    
    void upgrade() override;
    bool canUpgrade()  override;
    void resetLevel() override;
    
    int getAreaOfEffect() const override;
    void setAreaOfEffect(int area) override;
    
private:
    bool IsInAreaPosition(int x, int y, Player& player);
    int findCharactersInArea(int x, int y, GameField& field);
    bool damageObject(Type_cell cell, GameField& field, int x, int y, Player& player);
};

#endif