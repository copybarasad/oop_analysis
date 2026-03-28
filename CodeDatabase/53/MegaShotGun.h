#ifndef MEGASHOTGUN_H
#define MEGASHOTGUN_H

#include "SpellBase.h"
#include "IUpgradableSpell.h"
#include "IAreaSpell.h"

class MegaShotGun : public SpellBase, public IUpgradableSpell, public IAreaSpell {
private:
    int sizeZoneAttack;
    
public:
    MegaShotGun(int spell_level);
    
    bool use(int x, int y, GameField& field, Player& player) override;
    bool damageTheLiving(int x, int y, GameField& field, Player& player);
    
    void upgrade() override;
    bool canUpgrade() override;
    void resetLevel() override;
    
    int getAreaOfEffect() const override;
    void setAreaOfEffect(int area) override;
    
};

#endif