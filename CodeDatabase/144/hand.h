#ifndef HAND_H
#define HAND_H

#include "spell.h"
#include "spell_context.h"
#include <vector>
#include <memory>
#include <random>

class Game;

class Hand {
private:
	std::vector<std::unique_ptr<Spell>> spells;
	int maximumSize;

public:
	explicit Hand(int size);
	
	bool addSpell(std::unique_ptr<Spell> newSpell);
	bool castSpell(int spellIndex, SpellContext& context);
	void removeSpell(int spellIndex);
	
	int getSpellCount() const;
	const Spell* getSpell(int spellIndex) const;
	std::vector<std::unique_ptr<Spell>>& getSpells();
	const std::vector<std::unique_ptr<Spell>>& getSpells() const;
	int getMaximumSize() const;
	bool isFull() const;
	void removeRandomSpells(size_t count);
	
	std::unique_ptr<Spell> getRandomSpell();
	void displaySpells() const;
};

#endif