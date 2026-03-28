#include "Hand.h"
#include "SpellFactory.h"
#include "Spell.h"
#include "UpgradeSpell.h"
#include <iostream>

Hand::Hand(size_t capacity) : capacity_(capacity) {}

bool Hand::addSpell(const std::string& spellName) {
    if (spellName.empty()) return false;
    if (spells_.size() >= capacity_) return false;
    spells_.push_back(spellName);
    return true;
}

bool Hand::cast(size_t index, Field& field, Coord casterPos, std::optional<Coord> target, std::shared_ptr<Player> caster) {
    if (index >= spells_.size()) return false;
    std::string spellName = spells_[index];
    bool applyUpgrades = applyNext_ && !pending_.empty();
    std::shared_ptr<Spell> spell = SpellFactory::create(spellName, field, casterPos, target, caster, applyUpgrades, pending_, this);
    if (!spell) return false;
    bool used = spell->apply();
    if (used) {
        spells_.erase(spells_.begin() + index);
    }
    if (dynamic_cast<UpgradeSpell*>(spell.get())) {
        enhanceNext = true;
        return true;
    }
    return used;
}

void Hand::accumulateUpgrade(const UpgradeData& data) {
    pending_.addRange += data.addRange;
    pending_.addW += data.addW;
    pending_.addH += data.addH;
    pending_.addTrap += data.addTrap;
    pending_.addSummon += data.addSummon;
    applyNext_ = true;
}

void Hand::clearPendingUpgrade() {
    pending_ = UpgradeData{};
    applyNext_ = false;
}
