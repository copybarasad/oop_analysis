#ifndef TOWER_ACTION_CONTEXT_H
#define TOWER_ACTION_CONTEXT_H

class TowerActionContext {
public:
	virtual ~TowerActionContext() = default;

	virtual int getPlayerX() const = 0;
	virtual int getPlayerY() const = 0;
	virtual bool hasLineOfSight(int fromX, int fromY, int toX, int toY) const = 0;
	virtual int applyDamageToPlayer(int amount) = 0;
	
	// Метод для уведомления о событии атаки башни
	// Сущность только информирует, а контекст решает, как обработать это событие
	virtual void notifyTowerAttack(int towerX, int towerY, int damage, int playerHealth) = 0;
};

#endif


