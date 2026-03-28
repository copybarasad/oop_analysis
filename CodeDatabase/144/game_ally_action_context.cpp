#include "game_ally_action_context.h"
#include "game.h"
#include "game_logging.h"
#include <sstream>

GameAllyActionContext::GameAllyActionContext(Game& game)
	: game(game) {}

Field& GameAllyActionContext::getField() {
	return game.getField();
}

EnemyManager& GameAllyActionContext::getEnemyManager() {
	return game.getEnemyManager();
}

BuildingManager& GameAllyActionContext::getBuildingManager() {
	return game.getBuildingManager();
}

bool GameAllyActionContext::isCellOccupiedByEnemy(int x, int y) const {
	return game.isCellOccupiedByEnemy(x, y);
}

bool GameAllyActionContext::isCellOccupiedByBuilding(int x, int y) const {
	return game.getBuildingManager().isCellOccupiedByBuilding(x, y);
}

bool GameAllyActionContext::isCellOccupiedByAlly(int x, int y) const {
	return game.isCellOccupiedByAlly(x, y);
}

void GameAllyActionContext::rewardPlayerForEnemyKill(int points) {
	game.getPlayer().addScore(points);
}

void GameAllyActionContext::notifyAllyAttack(int allyX, int allyY, int damage, int enemyX, int enemyY) {
	// Game решает, логировать ли это событие
	std::ostringstream ss;
	ss << "Ally at (" << allyX << ", " << allyY 
	   << ") attacks enemy at (" << enemyX << ", " << enemyY 
	   << ") for " << damage << " damage";
	game.logEvent(GameEventType::Damage, ss.str());
}

