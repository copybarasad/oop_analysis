#ifndef ENEMY_TURN_HANDLER_H
#define ENEMY_TURN_HANDLER_H

#include "GameField.h"
#include "EntityManager.h"
#include "RandomGenerator.h"
#include "MovementHelper.h"
#include "TurnResults.h"
#include <functional>

class EnemyTurnHandler {
private:
	GameField& field;
	EntityManager& entity_manager;
	std::reference_wrapper<int> player_index_ref;
	RandomGenerator& rng;
	
	MovementHelper movement_helper;
	
	EnemyAction moveEnemyTowardsPlayer(int enemy_index, int target_player_index);
	SpawnEvent spawnEnemyNearBarracks(int barracks_index);

public:
	EnemyTurnHandler(GameField& game_field, EntityManager& em, int& player_idx, RandomGenerator& random_gen);
	
	EnemyTurnResult processEnemyTurn();
};

#endif







