#pragma once
#include <vector>
#include <memory>
#include <random>
#include "ISpell.hpp"

class SpellManager;

class PlayerHand {
public:
	explicit PlayerHand(size_t maxSize = 5);
	PlayerHand(const PlayerHand& other);
	PlayerHand(PlayerHand&& other) noexcept;
	PlayerHand& operator=(const PlayerHand& other);
	PlayerHand& operator=(PlayerHand&& other) noexcept;
	~PlayerHand() = default;

	bool addSpell(std::unique_ptr<ISpell> spell);
	bool useSpell(size_t index, SpellManager& spellManager, const Coords& casterPos, const Coords& target);
	const ISpell* getSpell(size_t index) const;
	size_t getSpellCount() const noexcept;
	size_t getMaxSize() const noexcept;
	bool hasSpace() const noexcept;
	bool addRandomSpell();
	void printHand() const;

private:
	std::vector<std::unique_ptr<ISpell>> spells;
	size_t maxSize;
	std::mt19937 rng;
	
	std::unique_ptr<ISpell> createRandomSpell();
};
