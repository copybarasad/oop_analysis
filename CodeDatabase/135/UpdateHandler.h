#ifndef UPDATE_HANDLER_H
#define UPDATE_HANDLER_H

#include "GameField.h"
#include "EntityManager.h"
#include "TrapManager.h"
#include "TurnResults.h"
#include <functional>

class UpdateHandler {
private:
	GameField& field;
	EntityManager& entity_manager;
	std::reference_wrapper<int> player_index_ref;
	
	TrapManager trap_manager;

public:
	UpdateHandler(GameField& game_field, EntityManager& em, int& player_idx);
	
	UpdateResult update();
};

#endif







