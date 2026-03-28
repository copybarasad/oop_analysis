#ifndef UPGRADESPELL_H
#define UPGRADESPELL_H

#include "ISpell.h"
#include "../Constants.h"

class UpgradeSpell: public ISpell{
private:
    int upgradePower;
    int cost;
public:
    UpgradeSpell();
    bool cast(GameState& gameState) override;
    void upgrade(int power) override;
    std::string getName() const override;
    int getCost() const override;
};

#endif