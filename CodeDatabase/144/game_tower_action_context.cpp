#include "game_tower_action_context.h"

#include "game.h"
#include "player.h"
#include "field.h"
#include "game_logging.h"
#include <sstream>

GameTowerActionContext::GameTowerActionContext(Game& game)
	: game(game) {}

int GameTowerActionContext::getPlayerX() const {
	return game.getPlayer().getX();
}

int GameTowerActionContext::getPlayerY() const {
	return game.getPlayer().getY();
}

bool GameTowerActionContext::hasLineOfSight(int fromX, int fromY, int toX, int toY) const {
	return game.getField().hasLineOfSight(fromX, fromY, toX, toY);
}

int GameTowerActionContext::applyDamageToPlayer(int amount) {
	Player& player = game.getPlayer();
	player.takeDamage(amount);
	return player.getHealth();
}

void GameTowerActionContext::notifyTowerAttack(int towerX, int towerY, int damage, int playerHealth) {
	// Game решает, логировать ли это событие
	std::ostringstream ss;
	ss << "Enemy tower at (" << towerX << ", " << towerY 
	   << ") shoots player for " << damage << " damage. Player HP: " << playerHealth;
	game.logEvent(GameEventType::Damage, ss.str());
}


