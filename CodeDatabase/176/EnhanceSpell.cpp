#include "EnhanceSpell.h"
#include "GameManager.h"
#include <iostream>

EnhanceSpell::EnhanceSpell(int cost) : manaCost(cost) {
    name = "Arcane Focus";
    description = "Enhances next spell";
}

bool EnhanceSpell::cast(GameManager& gameManager, int targetX, int targetY) {
    const_cast<Player&>(gameManager.getPlayer()).getEnhancementManager().addEnhancementStack();
    std::cout << name << " activated! Enhancement stacks: "
              << gameManager.getPlayer().getEnhancementManager().getEnhancementStacks() << "\n";
    return true;
}

std::string EnhanceSpell::getName() const {
    return name;
}

std::string EnhanceSpell::getDescription() const {
    return description;
}

int EnhanceSpell::getManaCost() const {
    return manaCost;
}

bool EnhanceSpell::requiresTarget() const {
    return false;
}
