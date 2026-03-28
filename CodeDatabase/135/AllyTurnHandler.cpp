#include "AllyTurnHandler.h"

AllyTurnHandler::AllyTurnHandler(GameField& game_field, EntityManager& em, int& player_idx, RandomGenerator& random_gen)
	: field(game_field),
	  entity_manager(em),
	  player_index_ref(player_idx),
	  rng(random_gen),
	  movement_helper(game_field, em),
	  ally_controller(game_field, em, movement_helper, random_gen) {}

AllyTurnResult AllyTurnHandler::processAllyTurn() {
	int& player_index = player_index_ref.get();
	AllyTurnResult result;
	
	if (player_index < 0 || !entity_manager.hasEntity(player_index)) {
		return result;
	}
	
	ally_controller.processAllyTurn(player_index);
	
	return result;
}
