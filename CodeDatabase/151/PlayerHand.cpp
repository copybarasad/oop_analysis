#include "PlayerHand.h"
#include "StraightSpell.h"
#include "SquareSpell.h"
#include "Trap.h"
#include "Summon.h"
#include "UpgradeSpell.h"
#include <cstdlib>
#include <iostream>

PlayerHand::PlayerHand(int maxSize) : maxSize(maxSize) {}

bool PlayerHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() < maxSize) {
        spells.push_back(std::move(spell));
        return true;
    }
    return false;
}

bool PlayerHand::removeSpell(int index) {
    if (index >= 0 && index < spells.size()) {
        spells.erase(spells.begin() + index);
        return true;
    }
    return false;
}

Spell* PlayerHand::getSpell(int index) const {
    if (index >= 0 && index < spells.size()) {
        return spells[index].get();
    }
    return nullptr;
}

bool PlayerHand::canCastSpell(int index, int playerMana) const {
    Spell* spell = getSpell(index);
    return spell && playerMana >= spell->getManaCost();
}

int PlayerHand::getSize() const {
    return spells.size();
}

int PlayerHand::getMaxSize() const {
    return maxSize;
}

bool PlayerHand::isFull() const {
    return spells.size() >= maxSize;
}

std::unique_ptr<Spell> PlayerHand::getRandomStarterSpell() {
    int choice = rand() % 5;
    switch (choice) {
    case 0: return std::make_unique<StraightSpell>();
    case 1: return std::make_unique<SquareSpell>();
    case 2: return std::make_unique<TrapSpell>();
    case 3: return std::make_unique<SummonSpell>();
    case 4: return std::make_unique<UpgradeSpell>();
    default: return std::make_unique<StraightSpell>();
    }
}