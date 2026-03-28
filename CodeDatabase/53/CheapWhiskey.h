#ifndef CHEAPWHISKEY_H
#define CHEAPWHISKEY_H

#include "SpellBase.h"
#include "IUpgradableSpell.h"
#include "IPlacementSpell.h"

class CheapWhiskey : public SpellBase, public IUpgradableSpell, public IPlacementSpell {
private:
    int placementRange;
    
public:
    CheapWhiskey(int spell_level);
    
    bool use(int x, int y, GameField& field, Player& player) override;
    
    void upgrade() override;
    bool canUpgrade() override;
    void resetLevel() override;

    bool isValidPlacement(int x, int y, const Player& player) const override;
};

#endif