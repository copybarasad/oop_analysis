#pragma once
#include <vector>
#include <memory>
#include <random>
#include "Coords.hpp"
#include "Trap.hpp"
#include "ISpell.hpp"

class Field;
class WorldManager;

class SpellManager {
public:
	explicit SpellManager(WorldManager& worldManager, size_t maxHandSize = 5);
	
	SpellManager(const SpellManager&) = delete;
	SpellManager& operator=(const SpellManager&) = delete;
	SpellManager(SpellManager&&) = default;
	SpellManager& operator=(SpellManager&&) = default;

	bool dealDamageAt(const Coords& pos, int damage);
	void dealAreaDamage(const Coords& pos, int width, int height, int damage);
	
	bool placeTrap(const Coords& pos, int damage);
	int triggerTrapAt(const Coords& pos);
	void removeInactiveTraps();
	
	bool addSpell(std::unique_ptr<ISpell> spell);
	
	bool addRandomSpell();
	
	bool useSpell(size_t index, const Coords& casterPos, const Coords& target);
	
	const ISpell* getSpell(size_t index) const;
	
	size_t getSpellCount() const noexcept;
	
	size_t getMaxHandSize() const noexcept;
	
	bool hasSpaceInHand() const noexcept;
	
	void printHand() const;
	
	int manhattan(const Coords& a, const Coords& b) const noexcept;
	
	bool isInRange(const Coords& casterPos, const Coords& target, int range) const noexcept;
	
	const std::vector<Trap>& getTraps() const noexcept { return traps; }
	const std::vector<std::unique_ptr<ISpell>>& getHand() const noexcept { return hand; }
	void clearHand() { hand.clear(); }
	void addSpellForLoad(std::unique_ptr<ISpell> spell) { hand.push_back(std::move(spell)); }

private:
	WorldManager& world;
	std::vector<Trap> traps;
	
	std::vector<std::unique_ptr<ISpell>> hand;
	size_t maxHandSize;
	std::mt19937 rng;
	
	std::unique_ptr<ISpell> createRandomSpell();
};
