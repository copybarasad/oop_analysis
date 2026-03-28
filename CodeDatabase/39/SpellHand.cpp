#include "SpellHand.h"
#include "Spell.h"

#include <stdexcept>

SpellHand::SpellHand(std::size_t max_size)
    : max_size_(max_size) {
    EnsureInvariants();
}

std::size_t SpellHand::GetMaxSize() const {
    return max_size_;
}

std::size_t SpellHand::GetSize() const {
    return spells_.size();
}

bool SpellHand::IsFull() const {
    return spells_.size() >= max_size_;
}

bool SpellHand::IsEmpty() const {
    return spells_.empty();
}

bool SpellHand::AddSpell(std::unique_ptr<Spell> spell) {
    if (!spell || IsFull()) {
        return false;
    }
    spells_.push_back(std::move(spell));
    EnsureInvariants();
    return true;
}

std::string SpellHand::GetSpellName(std::size_t index) const {
    if (index >= spells_.size()) {
        throw std::out_of_range("Spell index out of range");
    }
    return spells_[index]->GetName();
}

std::unique_ptr<Spell> SpellHand::TakeSpell(std::size_t index) {
    if (index >= spells_.size()) {
        throw std::out_of_range("Spell index out of range");
    }
    std::unique_ptr<Spell> result = std::move(spells_[index]);
    spells_.erase(spells_.begin() + static_cast<std::ptrdiff_t>(index));
    EnsureInvariants();
    return result;
}

void SpellHand::EnsureInvariants() const {
    if (max_size_ == 0) {
        throw std::logic_error("SpellHand max_size must be greater than zero");
    }
    if (spells_.size() > max_size_) {
        throw std::logic_error("SpellHand contains more spells than max_size");
    }
}
