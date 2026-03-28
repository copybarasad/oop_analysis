#include "SpellFactory.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhancementSpell.h"
#include <random>

int SpellFactory::calculateRadius(int fieldSize) {
	return fieldSize / 4;
}

std::unique_ptr<Spell> SpellFactory::CreateDirectDamageSpell(int fieldWidth, int fieldHeight) {
	int avgSize = (fieldWidth + fieldHeight) / 2;
	int radius = calculateRadius(avgSize);
	if (radius < 1) radius = 1;
	return std::make_unique<DirectDamageSpell>(30, radius);
}

std::unique_ptr<Spell> SpellFactory::CreateAreaDamageSpell(int fieldWidth, int fieldHeight) {
	int avgSize = (fieldWidth + fieldHeight) / 2;
	int radius = calculateRadius(avgSize);
	if (radius < 1) radius = 1;
	return std::make_unique<AreaDamageSpell>(20, radius, 2);
}

std::unique_ptr<Spell> SpellFactory::CreateTrapSpell(int fieldWidth, int fieldHeight) {
	int avgSize = (fieldWidth + fieldHeight) / 2;
	int radius = calculateRadius(avgSize);
	if (radius < 1) radius = 1;
	return std::make_unique<TrapSpell>(10, radius);
}

std::unique_ptr<Spell> SpellFactory::CreateRandomSpell(int fieldWidth, int fieldHeight) {
	std::random_device randomDevice;
	std::mt19937 randomGenerator(randomDevice());
	std::uniform_int_distribution<int> distribution(0, 4);
	
	int choice = distribution(randomGenerator);
	if (choice == 0) {
		return CreateDirectDamageSpell(fieldWidth, fieldHeight);
	} else if (choice == 1) {
		return CreateAreaDamageSpell(fieldWidth, fieldHeight);
	} else if (choice == 2) {
		return CreateTrapSpell(fieldWidth, fieldHeight);
	} else if (choice == 3) {
		int avgSize = (fieldWidth + fieldHeight) / 2;
		int radius = calculateRadius(avgSize);
		if (radius < 1) radius = 1;
		return std::make_unique<SummonSpell>(1, radius);
	} else {
		return std::make_unique<EnhancementSpell>();
	}
}
