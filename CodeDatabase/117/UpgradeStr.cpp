#include "UpgradeStr.h"
#include "../Spells/UpgradeSpell.h"
#include <memory>

bool UpgradeStr::execute(GameState& gameState) {
    std::unique_ptr<ISpell> spell = std::make_unique<UpgradeSpell>();
    spell->upgrade(gameState.getHand().getUpgradePower());
    gameState.getHand().setUpgradePower(0);
    return spell->cast(gameState);
}

std::string UpgradeStr::getName() const {
    return "Upgrade strategy";
}