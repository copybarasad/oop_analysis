#ifndef GAME_ENEMY_ACTION_CONTEXT_H
#define GAME_ENEMY_ACTION_CONTEXT_H

#include "enemy_action_context.h"

class Game;

class GameEnemyActionContext : public EnemyActionContext {
public:
	explicit GameEnemyActionContext(Game& game);

	void rewardPlayer(int points) override;

private:
	Game& game;
};

#endif


