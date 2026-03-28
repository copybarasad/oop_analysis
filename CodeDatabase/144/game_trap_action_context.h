#ifndef GAME_TRAP_ACTION_CONTEXT_H
#define GAME_TRAP_ACTION_CONTEXT_H

#include "trap_action_context.h"

class Game;

class GameTrapActionContext : public TrapActionContext {
public:
	explicit GameTrapActionContext(Game& game);
	~GameTrapActionContext();

	EnemyManager& getEnemyManager() override;
	EnemyActionContext& getEnemyActionContext() override;
	void onEnemyKilled() override;
	void notifyTrapTriggered(int trapX, int trapY, int damage, int enemyX, int enemyY) override;

private:
	Game& game;
	class GameEnemyActionContext* enemyActionContext;
};

#endif

