#ifndef SPELLFACTORY_H
#define SPELLFACTORY_H

#include "spell.h"
#include "spellStraightDamage.h"
#include "spellAreaDamage.h"
#include "spellTrap.h"
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

class SpellFactory
{
private:
	std::vector<Spell*> availableSpells;
public:
	SpellFactory();
	~SpellFactory();
	void cleanAvailableSpells();
	void initSpellsForHero();
	void initSpellsForEnemies();
	Spell* createRandomSpell();
	Spell* createSpell(int id);
	void showSpells();
	int getSize();
};

#endif