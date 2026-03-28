#include "SpellEnhancementManager.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include <iostream>

SpellEnhancementManager::SpellEnhancementManager() : enhancementStacks(0) {}

void SpellEnhancementManager::addEnhancementStack() {
    enhancementStacks++;
    std::cout << "Enhancement stack added! Total: " << enhancementStacks << "\n";
}

std::unique_ptr<Spell> SpellEnhancementManager::applyEnhancement(std::unique_ptr<Spell> spell) {
    if (enhancementStacks == 0 || !spell) {
        return spell;
    }
    
    std::cout << "Applying " << enhancementStacks << " enhancement stacks!\n";

    if (auto* directSpell = dynamic_cast<DirectDamageSpell*>(spell.get())) {
        auto enhancedSpell = std::make_unique<DirectDamageSpell>(
            directSpell->getDamage(),
            directSpell->getRange() + 2,
            directSpell->getManaCost());
        std::cout << "Direct damage range increased to " << enhancedSpell->getRange() << "!\n";
        resetEnhancement();
        return enhancedSpell;
    }
    else if (auto* areaSpell = dynamic_cast<AreaDamageSpell*>(spell.get())) {
        auto enhancedSpell = std::make_unique<AreaDamageSpell>(
            areaSpell->getDamage() + 5,
            areaSpell->getRange(),
            3,
            areaSpell->getManaCost());
        std::cout << "Area damage increased and size set to 3x3!\n";
        resetEnhancement();
        return enhancedSpell;
    }
    else if (auto* trapSpell = dynamic_cast<TrapSpell*>(spell.get())) {
        auto enhancedSpell = std::make_unique<TrapSpell>(
            trapSpell->getDamage() + 10,
            trapSpell->getRange(),
            trapSpell->getManaCost());
        std::cout << "Trap damage increased to " << enhancedSpell->getDamage() << "!\n";
        resetEnhancement();
        return enhancedSpell;
    }
    else if (auto* summonSpell = dynamic_cast<SummonSpell*>(spell.get())) {
        auto enhancedSpell = std::make_unique<SummonSpell>(
            summonSpell->getSummonCount() + 1,
            summonSpell->getRange(),
            summonSpell->getManaCost());
        std::cout << "Summon count increased to " << enhancedSpell->getSummonCount() << "!\n";
        resetEnhancement();
        return enhancedSpell;
    }
    resetEnhancement();
    return spell;
}

void SpellEnhancementManager::resetEnhancement() {
    enhancementStacks = 0;
}

int SpellEnhancementManager::getEnhancementStacks() const {
    return enhancementStacks;
}

bool SpellEnhancementManager::hasEnhancement() const {
    return enhancementStacks > 0;
}
