#include "UpgradeSpell.h"
#include "../Controllers/GameState.h"

UpgradeSpell::UpgradeSpell(): upgradePower(DEFAULT_UPGRADE_POWER), cost(UPGRADE_SPELL_COST) {}

int UpgradeSpell::getCost() const{
    return this->cost;
}

bool UpgradeSpell::cast(GameState& gameState){
    gameState.getHand().upgradeNextSpell(this->upgradePower);
    return true;
}

void UpgradeSpell::upgrade(int power){
    this->upgradePower += power;
}

std::string UpgradeSpell::getName() const{
    return "Upgrade spell";
}