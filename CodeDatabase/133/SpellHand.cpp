#include "SpellHand.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <numeric>
#include <random>
#include "SpellFactory.h"

SpellHand::SpellHand(int size, bool withStarter) : maxSize(size > 0 ? size : 1) {
	if (withStarter) {
		spells.push_back(SpellFactory::createRandomBasic());
	}
}

bool SpellHand::addSpell(std::unique_ptr<Spell> spell) {
	if (spells.size() >= static_cast<size_t>(maxSize)) {
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

bool SpellHand::replaceSpell(int index, std::unique_ptr<Spell> spell) {
	if (index < 0 || static_cast<size_t>(index) >= spells.size()) {
		return false;
	}
	spells[index] = std::move(spell);
	return true;
}

Spell* SpellHand::getSpell(int index) const {
	if (index < 0 || static_cast<size_t>(index) >= spells.size()) {
		return nullptr;
	}
	return spells[index].get();
}

int SpellHand::getSize() const {
	return static_cast<int>(spells.size());
}

int SpellHand::getMaxSize() const {
	return maxSize;
}

void SpellHand::increaseCapacity(int delta) {
	maxSize += delta;
	if (maxSize < 1) {
		maxSize = 1;
	}
}

void SpellHand::discardHalfRandomly() {
	if (spells.size() <= 1) {
		return;
	}

	const size_t removeCount = spells.size() / 2;
	std::vector<size_t> indexes(spells.size());
	std::iota(indexes.begin(), indexes.end(), 0);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::shuffle(indexes.begin(), indexes.end(), gen);

	std::vector<size_t> toRemove(indexes.begin(), indexes.begin() + static_cast<std::ptrdiff_t>(removeCount));
	std::sort(toRemove.begin(), toRemove.end(), std::greater<>());

	for (size_t index : toRemove) {
		spells.erase(spells.begin() + static_cast<std::ptrdiff_t>(index));
	}
}

std::vector<SpellId> SpellHand::getSpellIds() const {
	std::vector<SpellId> ids;
	ids.reserve(spells.size());
	for (const auto& spell : spells) {
		ids.push_back(spell->getId());
	}
	return ids;
}

void SpellHand::replaceSpells(std::vector<std::unique_ptr<Spell>>&& newSpells) {
	spells = std::move(newSpells);
}

void SpellHand::clear() {
	spells.clear();
}

void SpellHand::display() const {
	std::cout << "Рука заклинаний (" << spells.size() << "/" << maxSize << "):" << std::endl;
	for (size_t i = 0; i < spells.size(); ++i) {
		std::cout << i + 1 << ". " << spells[i]->getName() << std::endl;
	}
}