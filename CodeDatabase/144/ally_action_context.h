#ifndef ALLY_ACTION_CONTEXT_H
#define ALLY_ACTION_CONTEXT_H

class Field;
class EnemyManager;
class BuildingManager;

class AllyActionContext {
public:
	virtual ~AllyActionContext() = default;

	virtual Field& getField() = 0;
	virtual EnemyManager& getEnemyManager() = 0;
	virtual BuildingManager& getBuildingManager() = 0;
	virtual bool isCellOccupiedByEnemy(int x, int y) const = 0;
	virtual bool isCellOccupiedByBuilding(int x, int y) const = 0;
	virtual bool isCellOccupiedByAlly(int x, int y) const = 0;
	virtual void rewardPlayerForEnemyKill(int points) = 0;
	
	// Метод для уведомления о событии атаки союзника (сущность только информирует, контекст решает, логировать ли)
	virtual void notifyAllyAttack(int allyX, int allyY, int damage, int enemyX, int enemyY) = 0;
};

#endif

