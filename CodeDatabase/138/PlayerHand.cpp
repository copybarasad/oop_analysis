#include "PlayerHand.h"
#include "Spell.h"
#include <stdexcept>

PlayerHand::PlayerHand(size_t maxSize) 
	: maxHandSize(maxSize) {
	if (maxSize == 0) {
		throw std::invalid_argument("Hand size must be positive");
	}
	validateInvariant();
}

bool PlayerHand::AddSpell(std::unique_ptr<Spell> spell) {
	if (IsFull()) {
		return false;
	}
	if (!spell) {
		return false;
	}
	spells.push_back(std::move(spell));
	validateInvariant();
	return true;
}

bool PlayerHand::UseSpell(size_t index, GameField& field, int targetX, int targetY) {
	if (index >= spells.size()) {
		return false;
	}
	if (!spells[index]) {
		return false;
	}
	return spells[index]->Cast(field, targetX, targetY);
}

size_t PlayerHand::GetSpellCount() const {
	return spells.size();
}

std::string PlayerHand::GetSpellName(size_t index) const {
	if (index >= spells.size()) {
		throw std::out_of_range("Spell index out of range");
	}
	return spells[index]->GetName();
}

Spell* PlayerHand::GetSpellAt(size_t index) {
	if (index >= spells.size()) {
		return nullptr;
	}
	return spells[index].get();
}

void PlayerHand::RemoveSpell(size_t index) {
	if (index >= spells.size()) {
		throw std::out_of_range("Spell index out of range");
	}
	spells.erase(spells.begin() + index);
	validateInvariant();
}

bool PlayerHand::IsFull() const {
	return spells.size() >= maxHandSize;
}

void PlayerHand::validateInvariant() const {
	if (spells.size() > maxHandSize) {
		throw std::runtime_error("Hand size invariant violated");
	}
}
