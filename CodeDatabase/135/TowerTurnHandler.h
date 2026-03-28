#ifndef TOWER_TURN_HANDLER_H
#define TOWER_TURN_HANDLER_H

#include "GameField.h"
#include "EntityManager.h"
#include "TowerController.h"
#include "MovementHelper.h"
#include "TurnResults.h"
#include <functional>

class TowerTurnHandler {
private:
	GameField& field;
	EntityManager& entity_manager;
	std::reference_wrapper<int> player_index_ref;
	MovementHelper movement_helper;
	
	TowerController tower_controller;

public:
	TowerTurnHandler(GameField& game_field, EntityManager& em, int& player_idx);
	
	TowerTurnResult processTowerAttacks();
};

#endif
