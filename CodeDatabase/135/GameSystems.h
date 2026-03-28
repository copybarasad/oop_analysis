#ifndef GAME_SYSTEMS_H
#define GAME_SYSTEMS_H

#include "RandomGenerator.h"
#include "SpellFactory.h"
#include "EntityFactory.h"

class GameSystems {
private:
	RandomGenerator rng;
	SpellFactory spell_factory;
	EntityFactory entity_factory;

public:
	GameSystems();
	
	GameSystems(const GameSystems&) = delete;
	GameSystems& operator=(const GameSystems&) = delete;
	
	GameSystems(GameSystems&&) noexcept = default;
	GameSystems& operator=(GameSystems&&) noexcept = default;
	
	RandomGenerator& getRandomGenerator();
	const RandomGenerator& getRandomGenerator() const;
	
	SpellFactory& getSpellFactory();
	const SpellFactory& getSpellFactory() const;
	
	EntityFactory& getEntityFactory();
	const EntityFactory& getEntityFactory() const;
};

#endif







