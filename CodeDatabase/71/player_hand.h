#pragma once
#include "spell.h"
#include <vector>
#include <memory>
class Player;
class PlayingField;

class PlayerHand {
public:
	PlayerHand(size_t max_size);
	bool addSpell(std::unique_ptr<Spell> spell);
	void restoreSpell(std::unique_ptr<Spell> spell);

	SpellCastResult useSpell(size_t index, Player* caster, PlayingField& field);
	void show() const;
	bool isFull() const;
	size_t getSpellsCount() const;
	size_t getMaxSize() const; 
	const std::vector<std::unique_ptr<Spell>>& getSpells() const;

private:
	size_t max_size_;
	std::vector<std::unique_ptr<Spell>> spells_in_hand_;
};

