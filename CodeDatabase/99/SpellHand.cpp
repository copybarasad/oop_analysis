#include "../h/SpellHand.h"
#include "../h/DirectDamageSpell.h"
#include "../h/AreaDamageSpell.h"
#include <cstdlib>

using namespace std;

SpellHand::SpellHand(int maxSize) : maxSize(maxSize) {
	// Add one random spell to start
	spells.push_back(createRandomSpell());
}

int SpellHand::getMaxSize() const {
	return maxSize;
}

int SpellHand::getCurrentSize() const {
	return static_cast<int>(spells.size());
}

bool SpellHand::isFull() const {
	return getCurrentSize() >= maxSize;
}

bool SpellHand::isEmpty() const {
	return spells.empty();
}

// Adds spell to hand if not full
bool SpellHand::addSpell(unique_ptr<Spell> spell) {
	if (isFull()) {
		return false;
	}

	spells.push_back(std::move(spell));
	return true;
}

// Gets spell at index (non-owning pointer)
Spell* SpellHand::getSpell(int index) const {
	if (index < 0 || index >= getCurrentSize()) {
		return nullptr;
	}

	return spells[index].get();
}

// Removes spell at index after casting
void SpellHand::removeSpell(int index) {
	if (index < 0 || index >= getCurrentSize()) {
		return;
	}

	spells.erase(spells.begin() + index);
}

// Creates random spell for rewards
unique_ptr<Spell> SpellHand::createRandomSpell() const {
	int spellType = rand() % 2;

	if (spellType == 0) {
		// Direct damage spell
		int damage = 20 + rand() % 11;  // 20-30 damage
		int range = 3 + rand() % 3;     // 3-5 range
		return make_unique<DirectDamageSpell>(range, damage, "Fireball");
	} else {
		// Area damage spell
		int damage = 15 + rand() % 11;  // 15-25 damage
		int range = 5 + rand() % 3;     // 5-7 range
		return make_unique<AreaDamageSpell>(range, damage, "Meteor");
	}
}
