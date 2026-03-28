#ifndef PLAYER_H
#define PLAYER_H

#include "Unit.h"
#include "SpellHand.h"
#include <memory>

// Player class - represents the user-controlled character
class Player : public Unit {
private:
	std::unique_ptr<SpellHand> spellHand;

public:
	Player(int hp, int damage, int maxSpells = 5);
	
	// Spell hand management
	SpellHand* getSpellHand() const;
	bool canAffordSpell(int cost) const;
	void buySpell();
};

#endif // PLAYER_H
