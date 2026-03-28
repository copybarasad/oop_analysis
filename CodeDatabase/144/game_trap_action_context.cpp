#include "game_trap_action_context.h"
#include "game.h"
#include "game_logging.h"
#include <sstream>
#include "game_enemy_action_context.h"

GameTrapActionContext::GameTrapActionContext(Game& game)
	: game(game), enemyActionContext(new GameEnemyActionContext(game)) {}

GameTrapActionContext::~GameTrapActionContext() {
	delete enemyActionContext;
}

EnemyManager& GameTrapActionContext::getEnemyManager() {
	return game.getEnemyManager();
}

EnemyActionContext& GameTrapActionContext::getEnemyActionContext() {
	return *enemyActionContext;
}

void GameTrapActionContext::onEnemyKilled() {
	game.giveSpellForEnemyKill();
}

void GameTrapActionContext::notifyTrapTriggered(int trapX, int trapY, int damage, int enemyX, int enemyY) {
	// Game решает, логировать ли это событие
	std::ostringstream ss;
	ss << "Trap at (" << trapX << ", " << trapY 
	   << ") triggers on enemy at (" << enemyX << ", " << enemyY 
	   << ") for " << damage << " damage";
	game.logEvent(GameEventType::Damage, ss.str());
}

