#ifndef LEVEL_DIRECTOR_H
#define LEVEL_DIRECTOR_H

#include "LevelManager.h"
#include "LevelConfig.h"
#include "GameField.h"
#include "EntityManager.h"
#include "EntityFactory.h"
#include "ScoreSystem.h"
#include "Player.h"
#include <memory>

class RandomGenerator;
class SpellFactory;

class LevelDirector {
private:
	LevelManager level_manager;
	RandomGenerator& rng;
	SpellFactory& spell_factory;
	EntityFactory& entity_factory;
	
	void spawnEnemies(GameField& field, EntityManager& entity_manager, const LevelConfig& config);
	void spawnBarracks(GameField& field, EntityManager& entity_manager, const LevelConfig& config);

public:
	void reset();
	LevelDirector(RandomGenerator& random_gen, SpellFactory& spell_fact, EntityFactory& entity_fact);
	
	struct LevelInitResult {
		GameField field;
		EntityManager entity_manager;
		int player_index;
		int score_threshold;
	};
	
	LevelInitResult initializeLevel(int level);
	
	std::unique_ptr<Player> createPlayer();
	
	void advanceLevel();
	
	int getCurrentLevel() const;
	
	void setLevel(int level);
	
	void preparePlayerForNextLevel(Player& player);
};

#endif
