#include "PlayerSpellHand.h"
#include "DirectDamageSpell.h"
#include "AoESpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "BuffSpell.h"
#include "Constants.h"
#include "GameField.h"
#include "Position.h"
#include "Player.h"
#include "Game.h"
#include <random>
#include <chrono>

PlayerSpellHand::PlayerSpellHand(size_t maxHandSize) : maxHandSize(maxHandSize) {
    addSpell(generateRandomSpell());
}

bool PlayerSpellHand::addSpell(std::unique_ptr<Spell> spell) {
    if (spells.size() >= maxHandSize) {
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

bool PlayerSpellHand::useSpell(size_t index, GameField& field, const Position& target, Player& player, Game& game) {
    if (index >= spells.size()) return false;

    auto& spell = spells[index];

    bool result = spell->cast(field, target, player, game);

    if (!spell->isBuffSpell()) {
        resetBuffCount();
    }

    if (result) { 
        spells.erase(spells.begin() + index);
    }
    
    return result;
}

const std::vector<std::unique_ptr<Spell>>& PlayerSpellHand::getSpells() const {
    return spells;
}

size_t PlayerSpellHand::getMaxHandSize() const {
    return maxHandSize;
}

void PlayerSpellHand::increaseMaxHandSize(size_t amount) {
    maxHandSize += amount;
}

std::unique_ptr<Spell> PlayerSpellHand::generateRandomSpell() {
    std::mt19937 generator((time(nullptr)));;
    std::uniform_int_distribution<> dis(0, 4);
    switch (dis(generator)) {
        case 0: return std::make_unique<DirectDamageSpell>(DEFAULT_SPELL_DIRECT_DAMAGE, DEFAULT_SPELL_DIRECT_RANGE);
        case 1: return std::make_unique<AoESpell>(DEFAULT_SPELL_AOE_DAMAGE, DEFAULT_SPELL_AOE_AREA_SIZE);
        case 2: return std::make_unique<TrapSpell>(DEFAULT_TRAP_DAMAGE);
        case 3: return std::make_unique<SummonSpell>(1);
        case 4: return std::make_unique<BuffSpell>();
        default: return std::make_unique<DirectDamageSpell>(DEFAULT_SPELL_DIRECT_DAMAGE, DEFAULT_SPELL_DIRECT_RANGE);
    }
}

std::unique_ptr<Spell> PlayerSpellHand::createSpellByName(const std::string& name) {
    if (name == "Lightning Bolt") {
        return std::make_unique<DirectDamageSpell>(DEFAULT_SPELL_DIRECT_DAMAGE, DEFAULT_SPELL_DIRECT_RANGE);
    } else if (name == "Fireball") {
        return std::make_unique<AoESpell>(DEFAULT_SPELL_AOE_DAMAGE, DEFAULT_SPELL_AOE_AREA_SIZE);
    } else if (name == "Set Trap") {
        return std::make_unique<TrapSpell>(DEFAULT_TRAP_DAMAGE);
    } else if (name == "Summon Ally") {
        return std::make_unique<SummonSpell>(1);
    } else if (name == "Empower") {
        return std::make_unique<BuffSpell>();
    }
    return nullptr;
}

void PlayerSpellHand::removeHalfOfSpells() {
    if (spells.size() <= 1) return;
    
    size_t spellsToRemove = spells.size() / 2;
    std::mt19937 generator(static_cast<unsigned int>(time(nullptr)));
    
    for (size_t i = 0; i < spellsToRemove && !spells.empty(); i++) {
        std::uniform_int_distribution<size_t> distribution(0, spells.size() - 1);
        size_t indexToRemove = distribution(generator);
        spells.erase(spells.begin() + indexToRemove);
    }
}

void PlayerSpellHand::incrementBuffCount() {
    buffCount++;
}

int PlayerSpellHand::getBuffCount() const {
    return buffCount;
}

void PlayerSpellHand::resetBuffCount() {
    buffCount = 0;
}

void PlayerSpellHand::clear() {
    spells.clear();
    resetBuffCount();
}