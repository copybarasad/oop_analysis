#ifndef TURN_COORDINATOR_H
#define TURN_COORDINATOR_H

#include "PlayerTurnHandler.h"
#include "EnemyTurnHandler.h"
#include "AllyTurnHandler.h"
#include "TowerTurnHandler.h"
#include "UpdateHandler.h"
#include "GameField.h"
#include "EntityManager.h"
#include "SpellFactory.h"
#include "RandomGenerator.h"
#include "ScoreSystem.h"
#include <functional>

class TurnCoordinator {
private:
	PlayerTurnHandler player_handler;
	EnemyTurnHandler enemy_handler;
	AllyTurnHandler ally_handler;
	TowerTurnHandler tower_handler;
	UpdateHandler update_handler;

public:
	TurnCoordinator(GameField& game_field, EntityManager& em, int& player_idx,
	                SpellFactory& factory, RandomGenerator& random_gen, ScoreSystem& score_sys);
	
	PlayerTurnResult processPlayerTurn();
	EnemyTurnResult processEnemyTurn();
	AllyTurnResult processAllyTurn();
	TowerTurnResult processTowerAttacks();
	UpdateResult update();
	
	bool isPlayerAlive() const;
};

#endif







