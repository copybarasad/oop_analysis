#include "../h/Player.h"

using namespace std;

Player::Player(int hp, int damage, int maxSpells) 
	: Unit(hp, damage, 0), spellHand(make_unique<SpellHand>(maxSpells)) {
}

// Returns non-owning pointer to spell hand
SpellHand* Player::getSpellHand() const {
	return spellHand.get();
}

// Checks if player can afford spell (costs 10 score)
bool Player::canAffordSpell(int cost) const {
	return score >= cost;
}

// Buys new random spell
// Assumes checks have been done by caller
void Player::buySpell() {
	spellHand->addSpell(spellHand->createRandomSpell());
}
