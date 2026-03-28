#include "PlayerHand.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhancementSpell.h"
#include <random>

PlayerHand::PlayerHand(int maxSize, int killsForNewSpell)
    : maxSize(maxSize), selectedIndex(0), killCount(0), killsForNewSpell(killsForNewSpell) {
    addRandomSpell();
}

bool PlayerHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() >= static_cast<size_t>(maxSize)) {
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

bool PlayerHand::removeSpell(int index) {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        return false;
    }
    spells.erase(spells.begin() + index);
    if (selectedIndex >= static_cast<int>(spells.size()) && !spells.empty()) {
        selectedIndex = static_cast<int>(spells.size()) - 1;
    }
    return true;
}

Spell* PlayerHand::getSelectedSpell() const {
    if (spells.empty() || selectedIndex < 0 || selectedIndex >= static_cast<int>(spells.size())) {
        return nullptr;
    }
    return spells[selectedIndex].get();
}

void PlayerHand::selectNext() {
    if (!spells.empty()) {
        selectedIndex = (selectedIndex + 1) % static_cast<int>(spells.size());
    }
}

void PlayerHand::selectPrevious() {
    if (!spells.empty()) {
        selectedIndex = (selectedIndex - 1 + static_cast<int>(spells.size())) % static_cast<int>(spells.size());
    }
}

bool PlayerHand::selectSpell(int index) {
    if (index >= 0 && index < static_cast<int>(spells.size())) {
        selectedIndex = index;
        return true;
    }
    return false;
}

int PlayerHand::getSelectedIndex() const {
    return selectedIndex;
}

int PlayerHand::getSpellCount() const {
    return static_cast<int>(spells.size());
}

int PlayerHand::getMaxSize() const {
    return maxSize;
}

bool PlayerHand::hasSpace() const {
    return spells.size() < static_cast<size_t>(maxSize);
}

const Spell* PlayerHand::getSpell(int index) const {
    if (index >= 0 && index < static_cast<int>(spells.size())) {
        return spells[index].get();
    }
    return nullptr;
}

void PlayerHand::addRandomSpell() {
    if (!hasSpace()) return;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 4);

    int spellType = dis(gen);
    std::unique_ptr<Spell> newSpell;

    switch (spellType) {
    case 0:
        newSpell = std::make_unique<DirectDamageSpell>();
        break;
    case 1:
        newSpell = std::make_unique<AreaDamageSpell>();
        break;
    case 2:
        newSpell = std::make_unique<TrapSpell>();
        break;
    case 3:
        newSpell = std::make_unique<SummonSpell>();
        break;
    case 4:
        newSpell = std::make_unique<EnhancementSpell>();
        break;
    }

    addSpell(std::move(newSpell));
}

void PlayerHand::registerKill() {
    killCount++;
}

bool PlayerHand::checkAndRewardSpell() {
    if (killCount >= killsForNewSpell && hasSpace()) {
        killCount -= killsForNewSpell;
        addRandomSpell();
        return true;
    }
    return false;
}

EnhancementContext& PlayerHand::getEnhancement() {
    return enhancement;
}

void PlayerHand::resetEnhancement() {
    enhancement.reset();
}

int PlayerHand::getKillCount() const {
    return killCount;
}

int PlayerHand::getKillsUntilNextSpell() const {
    return killsForNewSpell - (killCount % killsForNewSpell);
}

void PlayerHand::clearAllSpells() {
    spells.clear();
    selectedIndex = 0;
}

void PlayerHand::setKillCount(int count) {
    killCount = count;
}

void PlayerHand::setSelectedIndex(int index) {
    if (index >= 0 && index < static_cast<int>(spells.size())) {
        selectedIndex = index;
    }
}
