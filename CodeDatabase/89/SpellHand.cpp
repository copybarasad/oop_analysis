#include "SpellHand.h"
#include "Spell.h"
#include "DirectDamageSpell.h"
#include "AoESpell.h"
#include "TrapSpell.h"
#include <random>
#include <iostream>

SpellHand::~SpellHand() = default;

static std::mt19937& getRNG() {
    static std::random_device rd;
    static std::mt19937 rng(rd());
    return rng;
}

SpellHand::SpellHand(int maxSize) : maxSize(maxSize) {
    if (maxSize > 0) {
        addSpell(createRandomSpell());
    }
}

bool SpellHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() >= static_cast<size_t>(maxSize)) {
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

std::unique_ptr<Spell> SpellHand::useSpell(int index) {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        return nullptr;
    }
    auto spell = std::move(spells[index]);
    spells.erase(spells.begin() + index);
    return spell;
}

size_t SpellHand::size() const {
    return spells.size();
}

std::string SpellHand::getSpellName(int index) const {
    if (index < 0 || index >= static_cast<int>(spells.size())) {
        return "Unknown";
    }
    return spells[index]->getName();
}

std::unique_ptr<Spell> SpellHand::createRandomSpell() {
    static auto& rng = getRNG();
    std::uniform_int_distribution<> dis(0, 2);
    switch (dis(rng)) {
        case 0: return std::make_unique<DirectDamageSpell>(20);
        case 1: return std::make_unique<AoESpell>(15);
        case 2: return std::make_unique<TrapSpell>(30);
        default: return std::make_unique<DirectDamageSpell>(20);
    }
}

void SpellHand::refillRandomSpell() {
    if (spells.size() < static_cast<size_t>(maxSize)) {
        addSpell(createRandomSpell());
    }
}