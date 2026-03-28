#include "05_spell_hand.h"
#include "03_direct_damage_spell.h"
#include "04_area_damage_spell.h"
#include <cstdlib>
#include <ctime>

SpellHand::SpellHand(int maxSize) : maxSize_(maxSize) {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
}

bool SpellHand::addSpell(std::shared_ptr<SpellCard> spell) {
    if (spells_.size() < maxSize_) {
        spells_.push_back(spell);
        return true;
    }
    return false;
}

bool SpellHand::useSpell(int index, const Position& casterPos, const Position& targetPos) {
    if (index < 0 || index >= spells_.size()) {
        return false;
    }

    return spells_[index]->cast(casterPos, targetPos);
}

std::shared_ptr<SpellCard> SpellHand::getSpell(int index) const {
    if (index < 0 || index >= spells_.size()) {
        return nullptr;
    }
    return spells_[index];
}

void SpellHand::removeSpell(int index) {
    if (index >= 0 && index < spells_.size()) {
        spells_.erase(spells_.begin() + index);
    }
}

int SpellHand::getSpellCount() const {
    return spells_.size();
}

int SpellHand::getMaxSize() const {
    return maxSize_;
}

bool SpellHand::isFull() const {
    return spells_.size() >= maxSize_;
}

std::shared_ptr<SpellCard> SpellHand::createRandomSpell() {
    int spellType = std::rand() % 2;

    if (spellType == 0) {
        // Прямой урон
        int damage = 20 + std::rand() % 20; // 20-40 урона
        int range = 3 + std::rand() % 3;    // 3-5 клеток
        return std::make_shared<DirectDamageSpell>(damage, range);
    }
    else {
        // Урон по площади
        int damage = 10 + std::rand() % 15; // 10-25 урона
        int range = 2 + std::rand() % 2;    // 2-3 клетки
        return std::make_shared<AreaDamageSpell>(damage, range);
    }
}