#include "game_enemy_movement_context.h"
#include "game.h"

GameEnemyMovementContext::GameEnemyMovementContext(const Game& game)
	: game(game) {}

bool GameEnemyMovementContext::isCellOccupiedByAlly(int x, int y) const {
	return game.isCellOccupiedByAlly(x, y);
}

const BuildingManager& GameEnemyMovementContext::getBuildingManager() const {
	return game.getBuildingManager();
}

