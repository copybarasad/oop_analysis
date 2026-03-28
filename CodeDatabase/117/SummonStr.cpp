#include "SummonStr.h"
#include "../Spells/SummonSpell.h"
#include <memory>

bool SummonStr::execute(GameState& gameState) {
    std::unique_ptr<ISpell> spell = std::make_unique<SummonSpell>();
    spell->upgrade(gameState.getHand().getUpgradePower());
    gameState.getHand().setUpgradePower(0);
    return spell->cast(gameState);
}

std::string SummonStr::getName() const {
    return "Summon strategy";
}