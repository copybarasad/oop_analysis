#include "GameRestartHandler.h"
#include "GameField.h"
#include "EntityManager.h"
#include "LevelDirector.h"
#include "ScoreSystem.h"
#include "RandomGenerator.h"

GameRestartHandler::GameRestartHandler(GameField& f, EntityManager& em, int& player_idx,
                                       LevelDirector& ld, ScoreSystem& ss, RandomGenerator& random_gen)
	: field(f), entity_manager(em), player_index(player_idx), 
	  level_director(ld), score_system(ss), rng(random_gen) {}

void GameRestartHandler::restartGame() {
	
	entity_manager = EntityManager();
	player_index = -1;
	
	level_director.reset();
	score_system = ScoreSystem();
	
	auto init_result = level_director.initializeLevel(1);
	field = std::move(init_result.field);
	entity_manager = std::move(init_result.entity_manager);
	player_index = init_result.player_index;
	score_system.setThreshold(init_result.score_threshold);
}
