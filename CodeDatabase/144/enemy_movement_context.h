#ifndef ENEMY_MOVEMENT_CONTEXT_H
#define ENEMY_MOVEMENT_CONTEXT_H

class BuildingManager;

class EnemyMovementContext {
public:
	virtual ~EnemyMovementContext() = default;

	virtual bool isCellOccupiedByAlly(int x, int y) const = 0;
	virtual const BuildingManager& getBuildingManager() const = 0;
};

#endif

