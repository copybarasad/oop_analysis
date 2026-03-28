#include "SpellHand.h"
#include "../SaveLoad.h"
#include <iostream>
#include <algorithm>
#include <random>
#include <chrono>

SpellHand::SpellHand(size_t maxSpells) : maxSize(maxSpells) {}

bool SpellHand::addSpell(std::unique_ptr<ISpell> spell) {
    if (isFull()) {
        return false;
    }
    spells.push_back(std::move(spell));
    return true;
}

bool SpellHand::removeSpell(int index) {
    if (index < 0 || static_cast<size_t>(index) >= spells.size()) {
        return false;
    }
    spells.erase(spells.begin() + index);
    return true;
}

bool SpellHand::castSpell(int index, Game& game, const Position& target) {
    if (index < 0 || static_cast<size_t>(index) >= spells.size()) {
        return false;
    }
    return spells[index]->cast(game, target);
}

const ISpell* SpellHand::getSpell(int index) const {
    if (index < 0 || static_cast<size_t>(index) >= spells.size()) {
        return nullptr;
    }
    return spells[index].get();
}

int SpellHand::getSpellCount() const {
    return spells.size();
}

size_t SpellHand::getMaxSize() const {
    return maxSize;
}

bool SpellHand::isFull() const {
    return spells.size() >= maxSize;
}

bool SpellHand::isEmpty() const {
    return spells.empty();
}

void SpellHand::removeRandomSpells(size_t count) {
    if (count >= spells.size()) {
        spells.clear();
        return;
    }

    std::vector<size_t> indices(spells.size());
    std::iota(indices.begin(), indices.end(), 0);

    std::random_device rd;
    std::mt19937 g(std::chrono::steady_clock::now().time_since_epoch().count());

    std::shuffle(indices.begin(), indices.end(), g);

    std::sort(indices.begin(), indices.begin() + count, std::greater<size_t>());

    for (size_t i = 0; i < count; ++i) {
        spells.erase(spells.begin() + indices[i]);
    }
}

void SpellHand::upgradeMaxSize(int amount) {
    if (amount <= 0)
        return;
    maxSize += amount;
}

void SpellHand::upgradeSpellAt(int index) {
    if (index < 0 || static_cast<size_t>(index) >= spells.size()) {
        std::cout << "Cannot upgrade spell: index out of range.\n";
        return;
    }

    
    std::unique_ptr<ISpell>& spell_ptr = spells[index];

    if (spell_ptr) {
        ISpell* spell_to_upgrade = spell_ptr.get();
        std::string old_name = spell_to_upgrade->getName();
        spell_to_upgrade->upgrade();

        std::cout << "Upgraded spell '" << old_name << "'! Its effect has increased.\n";
    } else {
        std::cout << "Cannot upgrade spell: slot is empty.\n";
    }
}

void SpellHand::displayHand() const {
    if (isEmpty()) {
        std::cout << "Spell hand is empty!\n";
        return;
    }

    std::cout << "=== SPELL HAND ===\n";
    for (size_t i = 0; i < spells.size(); ++i) {
        const ISpell* spell = spells[i].get();
        std::cout << i + 1 << ". " << spell->getSymbol() << " " << spell->getName()
                  << " (Cost: " << spell->getManaCost() << " mana, Range: " << spell->getRange() << ")\n";
                  std::cout << "    " << spell->getDescription() << "\n";
    }
    std::cout << "==================\n";
}

// === JSON ===
const std::vector<std::unique_ptr<ISpell>>& SpellHand::getSpells() const {
    return spells;
}

void SpellHand::loadState(const nlohmann::json& j) {
    const auto& spells_json = j.at("spells");
    std::vector<std::unique_ptr<ISpell>> new_spells;
    new_spells.reserve(spells_json.size());

    for (const auto& spell_json : spells_json) {
        std::unique_ptr<ISpell> spell;
        spell_json.get_to(spell);
        if (spell) {
            new_spells.push_back(std::move(spell));
        }
    }

    spells = std::move(new_spells);
}