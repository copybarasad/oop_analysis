#ifndef GAME_ALLY_ACTION_CONTEXT_H
#define GAME_ALLY_ACTION_CONTEXT_H

#include "ally_action_context.h"

class Game;

class GameAllyActionContext : public AllyActionContext {
public:
	explicit GameAllyActionContext(Game& game);

	Field& getField() override;
	EnemyManager& getEnemyManager() override;
	BuildingManager& getBuildingManager() override;
	bool isCellOccupiedByEnemy(int x, int y) const override;
	bool isCellOccupiedByBuilding(int x, int y) const override;
	bool isCellOccupiedByAlly(int x, int y) const override;
	void rewardPlayerForEnemyKill(int points) override;
	void notifyAllyAttack(int allyX, int allyY, int damage, int enemyX, int enemyY) override;

private:
	Game& game;
};

#endif

