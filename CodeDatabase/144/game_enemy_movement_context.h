#ifndef GAME_ENEMY_MOVEMENT_CONTEXT_H
#define GAME_ENEMY_MOVEMENT_CONTEXT_H

#include "enemy_movement_context.h"

class Game;

class GameEnemyMovementContext : public EnemyMovementContext {
public:
	explicit GameEnemyMovementContext(const Game& game);

	bool isCellOccupiedByAlly(int x, int y) const override;
	const BuildingManager& getBuildingManager() const override;

private:
	const Game& game;
};

#endif

