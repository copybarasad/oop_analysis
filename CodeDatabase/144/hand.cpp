#include "hand.h"
#include <iostream>

Hand::Hand(int size) : maximumSize(size) {}

bool Hand::addSpell(std::unique_ptr<Spell> newSpell) {
	if (spells.size() < maximumSize) {
		spells.push_back(std::move(newSpell));
		return true;
	}
	return false;
}

bool Hand::castSpell(int spellIndex, SpellContext& context) {
	if (spellIndex >= 0 && spellIndex < spells.size()) {
		return spells[spellIndex]->cast(context);
	}
	return false;
}

void Hand::removeSpell(int spellIndex) {
	if (spellIndex >= 0 && spellIndex < spells.size()) {
		spells.erase(spells.begin() + spellIndex);
	}
}

int Hand::getSpellCount() const { 
	return spells.size(); 
}

const Spell* Hand::getSpell(int spellIndex) const {
	if (spellIndex >= 0 && spellIndex < spells.size()) {
		return spells[spellIndex].get();
	}
	return nullptr;
}

std::vector<std::unique_ptr<Spell>>& Hand::getSpells() { 
	return spells; 
}

const std::vector<std::unique_ptr<Spell>>& Hand::getSpells() const { 
	return spells; 
}

int Hand::getMaximumSize() const { 
	return maximumSize; 
}

bool Hand::isFull() const {
	return spells.size() >= maximumSize;
}

void Hand::removeRandomSpells(size_t count) {
	if (spells.empty() || count == 0) {
		return;
	}
	count = std::min(count, spells.size());
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	for (size_t i = 0; i < count; ++i) {
		std::uniform_int_distribution<> distribution(0, spells.size() - 1);
		int index = distribution(generator);
		spells.erase(spells.begin() + index);
	}
}

std::unique_ptr<Spell> Hand::getRandomSpell() {
	if (spells.empty()) {
		return nullptr;
	}
	
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_int_distribution<> distribution(0, spells.size() - 1);
	
	int randomIndex = distribution(generator);
	auto selectedSpell = std::move(spells[randomIndex]);
	spells.erase(spells.begin() + randomIndex);
	return selectedSpell;
}

void Hand::displaySpells() const {
	if (spells.empty()) {
		std::cout << "No spells in hand.\n";
		return;
	}
	
	std::cout << "Spells in hand (" << spells.size() << "/" << maximumSize << "):\n";
	for (size_t i = 0; i < spells.size(); ++i) {
		const Spell* spell = spells[i].get();
		std::cout << i << ". " << spell->getName() 
				  << " - " << spell->getDescription()
				  << " (Mana: " << spell->getManaCost() << ")";
		
		if (spell->isEnhancement()) {
			std::cout << " [Level: " << spell->getEnhancementLevel() << "]";
		}
		
		std::cout << "\n";
	}
}

