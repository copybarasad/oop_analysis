#include "SpellManager.h"
#include <iostream>

SpellManager::SpellManager(int generationInterval)
    : m_generationCounter(0)
    , m_generationInterval(generationInterval) {
}

void SpellManager::update(Player& player) {
    m_generationCounter++;

    if (shouldGenerateSpell()) {
        generateNewSpell(player);
        m_generationCounter = 0;
    }
}

bool SpellManager::shouldGenerateSpell() const {
    return m_generationCounter >= m_generationInterval;
}

void SpellManager::generateNewSpell(Player& player) {
    if (player.getHand().getSpellCount() < player.getHand().getMaxSize()) {
        auto newSpell = SpellFactory::createRandomSpell();
        if (newSpell) {
            std::string spellName = newSpell->getName();
            if (player.addSpellToHand(std::move(newSpell))) {
                std::cout << "\n*** NEW SPELL ACQUIRED: " << spellName << " ***" << std::endl;
            }
        }
    }
}