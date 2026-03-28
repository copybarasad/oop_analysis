#include "PlayerHand.hpp"
#include "DirectDamageSpell.hpp"
#include "AreaDamageSpell.hpp"
#include "TrapSpell.hpp"
#include "SpellManager.hpp"
#include <iostream>
#include <chrono>

PlayerHand::PlayerHand(size_t maxSize)
	: maxSize(maxSize),
	  rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())) {
	addRandomSpell();
}

PlayerHand::PlayerHand(const PlayerHand& other)
	: maxSize(other.maxSize),
	  rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())) {
	spells.reserve(other.spells.size());
	for (const auto& spell : other.spells) {
		if (spell) {
			spells.push_back(spell->clone());
		}
	}
}

PlayerHand::PlayerHand(PlayerHand&& other) noexcept
	: spells(std::move(other.spells)),
	  maxSize(other.maxSize),
	  rng(std::move(other.rng)) {
	other.maxSize = 0;
}

PlayerHand& PlayerHand::operator=(const PlayerHand& other) {
	if (this != &other) {
		maxSize = other.maxSize;
		spells.clear();
		spells.reserve(other.spells.size());
		for (const auto& spell : other.spells) {
			if (spell) {
				spells.push_back(spell->clone());
			}
		}
	}
	return *this;
}

PlayerHand& PlayerHand::operator=(PlayerHand&& other) noexcept {
	if (this != &other) {
		spells = std::move(other.spells);
		maxSize = other.maxSize;
		rng = std::move(other.rng);
		other.maxSize = 0;
	}
	return *this;
}

bool PlayerHand::addSpell(std::unique_ptr<ISpell> spell) {
	if (!spell || spells.size() >= maxSize) {
		return false;
	}
	spells.push_back(std::move(spell));
	return true;
}

bool PlayerHand::useSpell(size_t index, SpellManager& spellManager, const Coords& casterPos, const Coords& target) {
	if (index >= spells.size() || !spells[index]) {
		return false;
	}
	
	bool used = spells[index]->cast(spellManager, casterPos, target);
	if (used) {
		spells.erase(spells.begin() + static_cast<ptrdiff_t>(index));
	}
	return used;
}

const ISpell* PlayerHand::getSpell(size_t index) const {
	if (index >= spells.size()) {
		return nullptr;
	}
	return spells[index].get();
}

size_t PlayerHand::getSpellCount() const noexcept {
	return spells.size();
}

size_t PlayerHand::getMaxSize() const noexcept {
	return maxSize;
}

bool PlayerHand::hasSpace() const noexcept {
	return spells.size() < maxSize;
}

std::unique_ptr<ISpell> PlayerHand::createRandomSpell() {
	std::uniform_int_distribution<int> dist(0, 2);
	int spellType = dist(rng);
	
	if (spellType == 0) {
		std::uniform_int_distribution<int> dmgDist(20, 40);
		std::uniform_int_distribution<int> rangeDist(3, 6);
		return std::make_unique<DirectDamageSpell>(dmgDist(rng), rangeDist(rng));
	} else if (spellType == 1) {
		std::uniform_int_distribution<int> dmgDist(10, 25);
		std::uniform_int_distribution<int> rangeDist(3, 5);
		return std::make_unique<AreaDamageSpell>(dmgDist(rng), rangeDist(rng));
	} else {
		std::uniform_int_distribution<int> dmgDist(25, 45);
		std::uniform_int_distribution<int> rangeDist(2, 4);
		return std::make_unique<TrapSpell>(dmgDist(rng), rangeDist(rng));
	}
}

bool PlayerHand::addRandomSpell() {
	if (!hasSpace()) {
		return false;
	}
	return addSpell(createRandomSpell());
}

void PlayerHand::printHand() const {
	std::cout << "=== Рука игрока (" << spells.size() << "/" << maxSize << ") ===\n";
	if (spells.empty()) {
		std::cout << "  (пусто)\n";
		return;
	}
	for (size_t i = 0; i < spells.size(); ++i) {
		const ISpell* spell = spells[i].get();
		std::cout << "  [" << i << "] " << spell->getName() 
				  << " - " << spell->getDescription() << "\n";
	}
}
