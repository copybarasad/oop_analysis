#include "TowerTurnHandler.h"

TowerTurnHandler::TowerTurnHandler(GameField& game_field, EntityManager& em, int& player_idx)
	: field(game_field),
	  entity_manager(em),
	  player_index_ref(player_idx),
	  movement_helper(game_field, em),
	  tower_controller(game_field, em, movement_helper) {}

TowerTurnResult TowerTurnHandler::processTowerAttacks() {
	int& player_index = player_index_ref.get();
	TowerTurnResult result;
	
	if (player_index < 0 || !entity_manager.hasEntity(player_index)) {
		return result;
	}
	
	tower_controller.processTowerAttacks(player_index);
	
	return result;
}
