#include "SpellManager.hpp"
#include "WorldManager.hpp"
#include "Field.hpp"
#include "DirectDamageSpell.hpp"
#include "AreaDamageSpell.hpp"
#include "TrapSpell.hpp"
#include <algorithm>
#include <iostream>
#include <chrono>

SpellManager::SpellManager(WorldManager& worldManager, size_t maxHandSize)
	: world(worldManager),
	  maxHandSize(maxHandSize),
	  rng(static_cast<unsigned>(std::chrono::steady_clock::now().time_since_epoch().count())) {
	addRandomSpell();
}


int SpellManager::manhattan(const Coords& a, const Coords& b) const noexcept {
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

bool SpellManager::isInRange(const Coords& casterPos, const Coords& target, int range) const noexcept {
	return manhattan(casterPos, target) <= range;
}

bool SpellManager::dealDamageAt(const Coords& pos, int damage) {
	Field& field = world.getField();
	if (!field.inBounds(pos)) return false;
	
	Cell& cell = field.cellAt(pos);
	OccupantType occupant = cell.getOccupant();
	
	if (occupant == OccupantType::Enemy) {
		return world.damageEnemyAt(pos, damage);
	} else if (occupant == OccupantType::Building) {
		return world.damageBuildingAt(pos, damage);
	} else if (occupant == OccupantType::Tower) {
		return world.damageTowerAt(pos, damage);
	}
	
	return false;
}

void SpellManager::dealAreaDamage(const Coords& pos, int width, int height, int damage) {
	for (int dx = 0; dx < width; ++dx) {
		for (int dy = 0; dy < height; ++dy) {
			Coords target{pos.x + dx, pos.y + dy};
			dealDamageAt(target, damage);
		}
	}
}


bool SpellManager::placeTrap(const Coords& pos, int damage) {
	Field& field = world.getField();
	if (!field.inBounds(pos)) return false;
	
	Cell& cell = field.cellAt(pos);
	if (!cell.isWalkable()) return false;
	if (cell.getOccupant() != OccupantType::None) return false;
	
	for (const auto& trap : traps) {
		if (trap.isActive() && trap.getPosition() == pos) {
			return false;
		}
	}
	
	traps.emplace_back(pos, damage);
	return true;
}

int SpellManager::triggerTrapAt(const Coords& pos) {
	for (auto& trap : traps) {
		if (trap.isActive() && trap.getPosition() == pos) {
			return trap.trigger();
		}
	}
	return 0;
}

void SpellManager::removeInactiveTraps() {
	traps.erase(
		std::remove_if(traps.begin(), traps.end(),
			[](const Trap& t) { return !t.isActive(); }),
		traps.end()
	);
}


bool SpellManager::addSpell(std::unique_ptr<ISpell> spell) {
	if (!spell || hand.size() >= maxHandSize) {
		return false;
	}
	hand.push_back(std::move(spell));
	return true;
}

std::unique_ptr<ISpell> SpellManager::createRandomSpell() {
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

bool SpellManager::addRandomSpell() {
	if (!hasSpaceInHand()) {
		return false;
	}
	return addSpell(createRandomSpell());
}

bool SpellManager::useSpell(size_t index, const Coords& casterPos, const Coords& target) {
	if (index >= hand.size() || !hand[index]) {
		return false;
	}
	
	int range = hand[index]->getRange();
	if (!isInRange(casterPos, target, range)) {
		return false;
	}
	
	bool used = hand[index]->cast(*this, casterPos, target);
	if (used) {
		hand.erase(hand.begin() + static_cast<ptrdiff_t>(index));
	}
	return used;
}

const ISpell* SpellManager::getSpell(size_t index) const {
	if (index >= hand.size()) {
		return nullptr;
	}
	return hand[index].get();
}

size_t SpellManager::getSpellCount() const noexcept {
	return hand.size();
}

size_t SpellManager::getMaxHandSize() const noexcept {
	return maxHandSize;
}

bool SpellManager::hasSpaceInHand() const noexcept {
	return hand.size() < maxHandSize;
}

void SpellManager::printHand() const {
	std::cout << "=== Рука игрока (" << hand.size() << "/" << maxHandSize << ") ===\n";
	if (hand.empty()) {
		std::cout << "  (пусто)\n";
		return;
	}
	for (size_t i = 0; i < hand.size(); ++i) {
		const ISpell* spell = hand[i].get();
		std::cout << "  [" << i << "] " << spell->getName() 
				  << " - " << spell->getDescription() << "\n";
	}
}
