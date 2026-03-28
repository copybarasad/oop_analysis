#ifndef SPELL_FACTORY_H
#define SPELL_FACTORY_H

#include "ISpell.h"
#include "RandomGenerator.h"
#include <memory>
#include <vector>
#include <functional>

class SpellFactory {
private:
	RandomGenerator& rng;
	using SpellCreator = std::function<std::unique_ptr<ISpell>()>;
	std::vector<SpellCreator> spell_creators;
	
	void registerDefaultSpells();

public:
	explicit SpellFactory(RandomGenerator& random_gen);
	
	std::unique_ptr<ISpell> createDirectDamage(int damage, int radius);
	std::unique_ptr<ISpell> createAreaDamage(int damage, int radius, int area_size = 2);
	std::unique_ptr<ISpell> createTrap(int damage, int radius);
	std::unique_ptr<ISpell> createSummon(int count, int ally_hp, int ally_damage);
	std::unique_ptr<ISpell> createEnhancement();
	std::unique_ptr<ISpell> createRandomSpell();
	
	void registerSpell(SpellCreator creator);
};

#endif
