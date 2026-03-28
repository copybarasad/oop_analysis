#ifndef ALLY_TURN_HANDLER_H
#define ALLY_TURN_HANDLER_H

#include "GameField.h"
#include "EntityManager.h"
#include "RandomGenerator.h"
#include "AllyController.h"
#include "MovementHelper.h"
#include "TurnResults.h"
#include <functional>

class AllyTurnHandler {
private:
	GameField& field;
	EntityManager& entity_manager;
	std::reference_wrapper<int> player_index_ref;
	RandomGenerator& rng;
	MovementHelper movement_helper;
	
	AllyController ally_controller;

public:
	AllyTurnHandler(GameField& game_field, EntityManager& em, int& player_idx, RandomGenerator& random_gen);
	
	AllyTurnResult processAllyTurn();
};

#endif
