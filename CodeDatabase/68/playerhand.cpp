#include "playerhand.h"
#include "directdamagespell.h"
#include "aoespell.h"
#include "trapspell.h"
#include "player.h"
#include <iostream>
#include <random>

PlayerHand::PlayerHand(int maxSize) : maxSize(maxSize) {
    // Start with one random spell as required by lab
    gainRandomSpell();
}

bool PlayerHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() < maxSize && spell) {
        spells.push_back(std::move(spell));
        return true;
    }
    return false;
}

void PlayerHand::removeSpell(int index) {
    if (index >= 0 && index < spells.size()) {
        spells.erase(spells.begin() + index);
    }
}

Spell* PlayerHand::getSpell(int index) const {
    if (index >= 0 && index < spells.size()) {
        return spells[index].get();
    }
    return nullptr;
}

void PlayerHand::gainRandomSpell() {
    if (isFull()) {
        std::cout << "Hand is full! Cannot gain new spell.\n";
        return;
    }
    
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 2);
    
    int spellType = dis(gen);
    std::unique_ptr<Spell> newSpell;
    
    switch (spellType) {
        case 0:
            newSpell = std::unique_ptr<Spell>(new DirectDamageSpell());
            break;
        case 1:
            newSpell = std::unique_ptr<Spell>(new AOESpell());
            break;
        case 2:
            newSpell = std::unique_ptr<Spell>(new TrapSpell());
            break;
    }
    
    if (newSpell) {
        spells.push_back(std::move(newSpell));
        std::cout << "Gained new spell: " << spells.back()->getName() << "\n";
    }
}

// NEW: Point-based spell purchase
bool PlayerHand::buySpell(Player* player, int spellCost) {
    if (!player) return false;
    
    if (isFull()) {
        std::cout << "Hand is full! Cannot buy new spell.\n";
        return false;
    }
    
    if (player->spendPoints(spellCost)) {
        gainRandomSpell();
        return true;
    }
    return false;
}

int PlayerHand::getSpellCount() const {
    return spells.size();
}

int PlayerHand::getMaxSize() const {
    return maxSize;
}

bool PlayerHand::isFull() const {
    return spells.size() >= maxSize;
}

void PlayerHand::displayHand() const {
    std::cout << "\nPlayer's Hand (" << spells.size() << "/" << maxSize << "):\n";
    for (size_t i = 0; i < spells.size(); i++) {
        std::cout << i << ". " << spells[i]->getName() 
                  << " | Damage: " << spells[i]->getDamage()
                  << " | Mana: " << spells[i]->getManaCost()
                  << " | Range: " << spells[i]->getRange() << "\n";
    }
    if (spells.empty()) {
        std::cout << "Hand is empty!\n";
    }
}