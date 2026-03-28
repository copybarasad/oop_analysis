#include "GameSystems.h"

GameSystems::GameSystems() 
	: rng(),
	  spell_factory(rng),
	  entity_factory() {
}

RandomGenerator& GameSystems::getRandomGenerator() {
	return rng;
}

const RandomGenerator& GameSystems::getRandomGenerator() const {
	return rng;
}

SpellFactory& GameSystems::getSpellFactory() {
	return spell_factory;
}

const SpellFactory& GameSystems::getSpellFactory() const {
	return spell_factory;
}

EntityFactory& GameSystems::getEntityFactory() {
	return entity_factory;
}

const EntityFactory& GameSystems::getEntityFactory() const {
	return entity_factory;
}







