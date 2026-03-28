#include "SpellFactory.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhancementSpell.h"

SpellFactory::SpellFactory(RandomGenerator& random_gen) : rng(random_gen) {
	registerDefaultSpells();
}

void SpellFactory::registerDefaultSpells() {
	registerSpell([this]() { return createDirectDamage(15, 5); });
	registerSpell([this]() { return createAreaDamage(10, 4, 2); });
	registerSpell([this]() { return createTrap(20, 5); });
	registerSpell([this]() { return createSummon(1, 25, 5); });
	registerSpell([this]() { return createEnhancement(); });
}

std::unique_ptr<ISpell> SpellFactory::createDirectDamage(int damage, int radius) {
	return std::make_unique<DirectDamageSpell>(damage, radius);
}

std::unique_ptr<ISpell> SpellFactory::createAreaDamage(int damage, int radius, int area_size) {
	return std::make_unique<AreaDamageSpell>(damage, radius, area_size);
}

std::unique_ptr<ISpell> SpellFactory::createTrap(int damage, int radius) {
	return std::make_unique<TrapSpell>(damage, radius);
}

std::unique_ptr<ISpell> SpellFactory::createSummon(int count, int ally_hp, int ally_damage) {
	return std::make_unique<SummonSpell>(count, ally_hp, ally_damage);
}

std::unique_ptr<ISpell> SpellFactory::createEnhancement() {
	return std::make_unique<EnhancementSpell>();
}

std::unique_ptr<ISpell> SpellFactory::createRandomSpell() {
	if (spell_creators.empty()) {
		return createDirectDamage(15, 5);
	}
	
	int index = rng.getInt(0, static_cast<int>(spell_creators.size()) - 1);
	return spell_creators[index]();
}

void SpellFactory::registerSpell(SpellCreator creator) {
	spell_creators.push_back(creator);
}
