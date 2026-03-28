#include "spellFactory.h"

SpellFactory::SpellFactory(){}
SpellFactory::~SpellFactory()
{
	cleanAvailableSpells();
}

void SpellFactory::cleanAvailableSpells()
{
    for (size_t i = 0; i < availableSpells.size(); ++i) {
        if (availableSpells[i] != nullptr) 
        { 
            delete availableSpells[i];
            availableSpells[i] = nullptr;
        }
    }
    availableSpells.clear();
}

void SpellFactory::initSpellsForHero()
{
	cleanAvailableSpells();
	availableSpells.push_back(new SpellStraightDamage(10, 3, 15, "Zeus's Sword"));
	availableSpells.push_back(new SpellAreaDamage(4, 2, 15, "Prometheus's Power"));
	availableSpells.push_back(new SpellTrap(15, 10, "Mystery Pit"));
	availableSpells.push_back(new SpellTrap(25, 20, "Joker's Trap"));
	availableSpells.push_back(new SpellAreaDamage(15, 6, 30, "Frog Rain"));
	availableSpells.push_back(new SpellStraightDamage(35, 4, 35, "Sindbad the Sailor's Power"));
}

void SpellFactory::initSpellsForEnemies()
{
	cleanAvailableSpells();
	availableSpells.push_back(new SpellStraightDamage(5, 3, 0, "Zeus's Dagger"));
	availableSpells.push_back(new SpellStraightDamage(7, 3, 0, "Apollo's Arrow"));
	availableSpells.push_back(new SpellStraightDamage(5, 3, 0, "Dionysus's Wine Glass Fragments"));
}

Spell* SpellFactory::createRandomSpell()
{
    srand(time(NULL));
    int id = rand() % availableSpells.size();
    return createSpell(id);
}

Spell* SpellFactory::createSpell(int id)
{
	if(id >= 0 and id < availableSpells.size())
	{
		return availableSpells[id]->clone();
	} else
	{
		return nullptr;
	}
}

void SpellFactory::showSpells()
{
	for (size_t i = 0; i < availableSpells.size(); ++i)
	{
		std::cout << std::format("{}: ", i);
		availableSpells[i]->showSpellStats();
	}
}

int SpellFactory::getSize()
{
	return availableSpells.size();
}