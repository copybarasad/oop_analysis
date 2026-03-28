#include "game_enemy_action_context.h"

#include "game.h"
#include "player.h"

GameEnemyActionContext::GameEnemyActionContext(Game& game)
	: game(game) {}

void GameEnemyActionContext::rewardPlayer(int points) {
	game.getPlayer().addScore(points);
}


