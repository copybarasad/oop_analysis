#include "spell_factory.h"
#include "direct_damage_spell.h"
#include "area_damage_spell.h"
#include <random>

std::unique_ptr<Spell> SpellFactory::createSpell(SpellType type) {
	switch (type) {
	case SpellType::DirectDamage:
		return std::make_unique<DirectDamageSpell>(25, 5.0f);
	case SpellType::AreaDamage:
		return std::make_unique<AreaDamageSpell>(10, 4.0f); 
	default:
		return nullptr;
	}
}

std::unique_ptr<Spell> SpellFactory::createRandomSpell() {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distrib(0, static_cast<int>(SpellType::COUNT) - 1);

	SpellType random_type = static_cast<SpellType>(distrib(gen));
	return createSpell(random_type);
}

std::string SpellFactory::getSpellName(SpellType type) {
	switch (type) {
	case SpellType::DirectDamage:
		return "Заклинание прямого урона";
	case SpellType::AreaDamage:
		return "Заклинание урона по площади";
	default:
		return "Неизвестное заклинание";
	}
}

int SpellFactory::getSpellCost(SpellType type) {
	switch (type) {
	case SpellType::DirectDamage:
		return 25; 
	case SpellType::AreaDamage:
		return 20; 
	default:
		return 9999; 
	}
}