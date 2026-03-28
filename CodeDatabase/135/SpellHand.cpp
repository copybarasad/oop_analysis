#include "SpellHand.h"
#include <algorithm>

SpellHand::SpellHand(size_t hand_size) : max_size(hand_size) {
	spells.reserve(max_size);
}

bool SpellHand::addSpell(std::unique_ptr<ISpell> spell) {
	if (!spell || spells.size() >= max_size) {
		return false;
	}
	spells.push_back(std::move(spell));
	return true;
}

ISpell* SpellHand::getSpell(size_t index) const {
	if (index >= spells.size()) {
		return nullptr;
	}
	return spells[index].get();
}

std::unique_ptr<ISpell> SpellHand::useSpell(size_t index) {
	if (index >= spells.size()) {
		return nullptr;
	}
	
	std::unique_ptr<ISpell> spell = std::move(spells[index]);
	spells.erase(spells.begin() + static_cast<long>(index));
	return spell;
}

bool SpellHand::hasSpace() const {
	return spells.size() < max_size;
}

size_t SpellHand::getCurrentSize() const {
	return spells.size();
}

size_t SpellHand::getMaxSize() const {
	return max_size;
}

bool SpellHand::isEmpty() const {
	return spells.empty();
}

void SpellHand::increaseMaxSize(size_t amount) {
	max_size += amount;
	spells.reserve(max_size);
}

void SpellHand::removeHalfSpells() {
	size_t spells_to_remove = spells.size() / 2;
	
	for (size_t i = 0; i < spells_to_remove && !spells.empty(); ++i) {
		size_t random_index = static_cast<size_t>(rand()) % spells.size();
		spells.erase(spells.begin() + static_cast<long>(random_index));
	}
}
