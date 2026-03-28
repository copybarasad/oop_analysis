#ifndef GAME_TOWER_ACTION_CONTEXT_H
#define GAME_TOWER_ACTION_CONTEXT_H

#include "tower_action_context.h"

class Game;

class GameTowerActionContext : public TowerActionContext {
public:
	explicit GameTowerActionContext(Game& game);

	int getPlayerX() const override;
	int getPlayerY() const override;
	bool hasLineOfSight(int fromX, int fromY, int toX, int toY) const override;
	int applyDamageToPlayer(int amount) override;
	void notifyTowerAttack(int towerX, int towerY, int damage, int playerHealth) override;

private:
	Game& game;
};

#endif


