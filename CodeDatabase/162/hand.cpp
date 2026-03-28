#include "hand.h"
#include "spell.h"
#include <iostream>

Hand::Hand(int maxSize) : maxSize_(maxSize) {}

void Hand::AddSpell(std::unique_ptr<Spell> spell) {
    if (!IsFull()) {
        spells_.push_back(std::move(spell));
    }
}

bool Hand::UseSpell(int index, Player& player, Field& field, int targetX, int targetY) {
    if (index < 0 || index >= spells_.size()) return false;
    bool success = spells_[index]->Use(player, field, targetX, targetY);
    if (success) {
        spells_.erase(spells_.begin() + index);
    }
    return success;
}

void Hand::Display() const {
    for (size_t i = 0; i < spells_.size(); ++i) {
        std::cout << i << ": " << spells_[i]->GetName() << "\n";
    }
}

void Hand::RemoveSpell(int index) {
    if (index >= 0 && index < spells_.size()) {
        spells_.erase(spells_.begin() + index);
    }
}

std::vector<std::string> Hand::GetSpellTypes() const {
    std::vector<std::string> types;
    for (const auto& s : spells_) {
        types.push_back(s->GetType());
    }
    return types;
}

void Hand::LoadFromTypes(const std::vector<std::string>& types) {
    spells_.clear();
    for (const auto& type : types) {
        if (type == "DirectDamage") spells_.push_back(std::make_unique<DirectDamageSpell>());
        else if (type == "AreaDamage") spells_.push_back(std::make_unique<AreaDamageSpell>());
        else if (type == "Trap") spells_.push_back(std::make_unique<TrapSpell>());
        else if (type == "Summon") spells_.push_back(std::make_unique<SummonSpell>());
        else if (type == "Buff") spells_.push_back(std::make_unique<BuffSpell>());
    }
}