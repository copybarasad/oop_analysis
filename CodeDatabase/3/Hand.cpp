#include "GameObjects/Spells/Hand.hpp"
#include "GameObjects/Spells/Target.hpp"
#include "GameObjects/Spells/Area.hpp"
#include "GameObjects/Spells/Trap.hpp"

Hand::Hand(HandDTO hand) {
    this->targetLvl = hand.targetLvl;
    this->areaLvl = hand.areaLvl;
    this->trapLvl = hand.trapLvl;
    this->maxSize = hand.capacity;
    for (auto& s : hand.spells) {
        spells.push_back(createSpell(s));
    }
}

HandDTO Hand::toDTO() {
    HandDTO dto;
    dto.areaLvl = areaLvl;
    dto.capacity = maxSize;
    dto.targetLvl = targetLvl;
    dto.trapLvl = trapLvl;
    for (auto& s : spells) {
        dto.spells.push_back(s->getSpellType());
    }
    return dto;
}

void Hand::addRandomSpell() {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist2(0, 2);

    if (spells.size() >= maxSize) return;

    SpellType type = SpellType(dist2(rng));
    spells.push_back(createSpell(type));
}

Spell* Hand::getSpellByIndex(int index) {
    if (index < 0 || index >= spells.size()) return nullptr;
    return spells[index].get();
}

int Hand::size() {
    return this->spells.size();
}

void Hand::deleteSpellByIndex(int index) {
    if (index < 0 || index >= spells.size()) return;
    spells.erase(spells.begin() + index);
}

std::unique_ptr<Spell> Hand::createSpell(SpellType spell) {
    switch (spell) {
        case SpellType::TargetSpell:
            return std::make_unique<Target>(targetLvl);
        case SpellType::AoESpell:
            return std::make_unique<Area>(areaLvl);
        case SpellType::TrapSpell:
            return std::make_unique<Trap>(trapLvl);
        default:
            return nullptr;
    }
}


void Hand::shakeHand() {
    if (spells.empty()) return;

    int removeCount = spells.size() / 2;

    std::vector<int> indexes(spells.size());
    for (int i = 0; i < spells.size(); ++i) {
        indexes[i] = i;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(indexes.begin(), indexes.end(), gen);


    for (int i = 0; i <= removeCount; ++i) {
        this->deleteSpellByIndex(indexes[i]);
    }
}


void Hand::upTargetLvl() {
    targetLvl++;
}

void Hand::upAreaLvl() {
    areaLvl++;
}

void Hand::upTrapLvl() {
    trapLvl++;
}
