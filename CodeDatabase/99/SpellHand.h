#ifndef SPELLHAND_H
#define SPELLHAND_H

#include "Spell.h"
#include <vector>
#include <memory>

// Player's hand of spells
// Manages collection of spells player can cast
class SpellHand {
private:
	std::vector<std::unique_ptr<Spell>> spells;
	int maxSize;

public:
	explicit SpellHand(int maxSize);

	// Getters
	int getMaxSize() const;
	int getCurrentSize() const;
	bool isFull() const;
	bool isEmpty() const;

	// Spell management
	bool addSpell(std::unique_ptr<Spell> spell);
	Spell* getSpell(int index) const;
	void removeSpell(int index);

	// Create random spell for rewards
	std::unique_ptr<Spell> createRandomSpell() const;
};

#endif // SPELLHAND_H
