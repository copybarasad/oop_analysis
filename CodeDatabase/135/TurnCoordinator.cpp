#include "TurnCoordinator.h"

TurnCoordinator::TurnCoordinator(GameField& game_field, EntityManager& em, int& player_idx,
                                   SpellFactory& factory, RandomGenerator& random_gen, ScoreSystem& score_sys)
	: player_handler(game_field, em, player_idx, factory, random_gen, score_sys),
	  enemy_handler(game_field, em, player_idx, random_gen),
	  ally_handler(game_field, em, player_idx, random_gen),
	  tower_handler(game_field, em, player_idx),
	  update_handler(game_field, em, player_idx) {}

PlayerTurnResult TurnCoordinator::processPlayerTurn() {
	return player_handler.processPlayerTurn();
}

EnemyTurnResult TurnCoordinator::processEnemyTurn() {
	return enemy_handler.processEnemyTurn();
}

AllyTurnResult TurnCoordinator::processAllyTurn() {
	return ally_handler.processAllyTurn();
}

TowerTurnResult TurnCoordinator::processTowerAttacks() {
	return tower_handler.processTowerAttacks();
}

UpdateResult TurnCoordinator::update() {
	return update_handler.update();
}

bool TurnCoordinator::isPlayerAlive() const {
	return player_handler.isPlayerAlive();
}







