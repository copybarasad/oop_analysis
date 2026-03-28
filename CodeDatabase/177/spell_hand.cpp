#include "spell_hand.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include <random>
#include <stdexcept>

SpellHand::SpellHand(int handSize) : maxSize(handSize), enemiesDefeatedCount(0) {
    if (handSize <= 0) throw std::invalid_argument("Hand size must be positive");
    validateHandSize();
}

void SpellHand::validateHandSize() const {
    if (static_cast<int>(spells.size()) > maxSize) throw std::runtime_error("Hand size exceeds maximum capacity");
}

bool SpellHand::addSpell(std::unique_ptr<SpellCard> spell) {
    if (isFull()) return false;
    spells.push_back(std::move(spell));
    validateHandSize();
    return true;
}

bool SpellHand::removeSpell(int index) {
    if (index < 0 || index >= static_cast<int>(spells.size())) return false;
    spells.erase(spells.begin() + index);
    return true;
}

void SpellHand::castSpell(int index) {
    if (index < 0 || index >= static_cast<int>(spells.size())) throw std::out_of_range("Invalid spell index");
    spells[index]->cast();
}

SpellCard* SpellHand::getSpell(int index) const {
    if (index < 0 || index >= static_cast<int>(spells.size())) return nullptr;
    return spells[index].get();
}

int SpellHand::getSpellCount() const { return static_cast<int>(spells.size()); }
int SpellHand::getMaxSize() const { return maxSize; }
bool SpellHand::isFull() const { return static_cast<int>(spells.size()) >= maxSize; }

void SpellHand::onEnemyDefeated() {
    ++enemiesDefeatedCount;
    if (enemiesDefeatedCount >= ENEMIES_FOR_NEW_SPELL && !isFull()) {
        initializeWithRandomSpell();
        enemiesDefeatedCount = 0;
    }
}

bool SpellHand::canBuySpell(int playerScore) const { return !isFull() && playerScore >= 50; }

void SpellHand::buySpell(int& playerScore) {
    if (!canBuySpell(playerScore)) throw std::runtime_error("Cannot buy spell - hand full or insufficient score");
    if (playerScore < 50) throw std::invalid_argument("Insufficient score to buy spell");
    
    playerScore -= 50;
    initializeWithRandomSpell();
}

void SpellHand::initializeWithRandomSpell() {
    if (isFull()) return;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, 2);
    
    std::unique_ptr<SpellCard> spell;
    switch (dist(gen)) {
        case 0: spell = std::make_unique<DirectDamageSpell>(); break;
        case 1: spell = std::make_unique<AreaDamageSpell>(); break;
        case 2: spell = std::make_unique<TrapSpell>(); break;
    }
    
    addSpell(std::move(spell));
}

bool SpellHand::removeRandomSpell() {
    if (spells.empty()) return false;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, spells.size() - 1);
    return removeSpell(dist(gen));
}