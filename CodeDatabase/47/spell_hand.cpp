#include "spell_hand.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include "trap_spell.h"
#include <cstdlib>
#include <iostream>

SpellHand::SpellHand(int handSize)
    : maxSize(handSize), currentMana(10), maxMana(10) {
}

bool SpellHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() < maxSize) {
        spells.push_back(std::move(spell));
        return true;
    }
    return false;
}

bool SpellHand::castSpell(int index, Game& game, int targetX, int targetY) {
    if (canCastSpell(index)) {
        if (spells[index]->isValidTarget(game, targetX, targetY)) {
            consumeMana(spells[index]->getManaCost());
            return spells[index]->cast(game, targetX, targetY);
        } 
        else {
            return false;
        }
    }
    return false;
}

bool SpellHand::canCastSpell(int index) const {
    return currentMana >= spells[index]->getManaCost();
}

const std::vector<std::unique_ptr<Spell>>& SpellHand::getSpells() const {
    return spells;
}

int SpellHand::getSpellCount() const {
    return spells.size();
}

int SpellHand::getMaxSize() const {
    return maxSize;
}

bool SpellHand::isFull() const {
    return spells.size() >= maxSize;
}

int SpellHand::getCurrentMana() const {
    return currentMana;
}

int SpellHand::getMaxMana() const {
    return maxMana;
}

void SpellHand::restoreMana(int amount) {
    currentMana += amount;
    if (currentMana > maxMana) {
        currentMana = maxMana;
    }
}

void SpellHand::setCurrentMana(int mana) {
    currentMana = mana;
    if (currentMana > maxMana) {
        currentMana = maxMana;
    }
    if (currentMana < 0) {
           currentMana = 0;
    }
}

void SpellHand::consumeMana(int amount) {
    currentMana -= amount;
}

void SpellHand::addRandomSpell() {
    if (isFull()) return;
    
    std::vector<std::string> existingSpellTypes;
    for (const auto& spell : spells) {
        existingSpellTypes.push_back(spell->getName());
    }
    
    std::vector<std::unique_ptr<Spell>> availableSpells;
    availableSpells.push_back(std::make_unique<DirectDamageSpell>(3, 4));
    availableSpells.push_back(std::make_unique<AreaDamageSpell>(2, 3));
    availableSpells.push_back(std::make_unique<TrapSpell>(2, 2));
    
    auto it = availableSpells.begin();
    while (it != availableSpells.end()) {
        bool alreadyExists = false;
        for (const auto& existingType : existingSpellTypes) {
            if ((*it)->getName() == existingType) {
                alreadyExists = true;
                break;
            }
        }
        
        if (alreadyExists) {
            it = availableSpells.erase(it);
        } 
        else {
            ++it;
        }
    }
    
    if (!availableSpells.empty()) {
        int randomIndex = std::rand() % availableSpells.size();
        addSpell(std::move(availableSpells[randomIndex]));
    }
}

void SpellHand::increaseMaxMana(int amount) {
    maxMana += amount;
    currentMana += amount;
}

void SpellHand::enhanceAllSpells() {
    for (auto& spell : spells) {
        spell->increaseDamage(1);
    }
}

void SpellHand::removeRandomSpells(int count) {
    if (spells.empty() || count <= 0) return;
    
    int spellsToRemove = std::min(count, static_cast<int>(spells.size()));
    
    for (int i = 0; i < spellsToRemove; ++i) {
        if (!spells.empty()) {
            int randomIndex = std::rand() % spells.size();
            spells.erase(spells.begin() + randomIndex);
        }
    }
}

void SpellHand::clearSpells() {
    spells.clear();
}