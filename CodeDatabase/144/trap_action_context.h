#ifndef TRAP_ACTION_CONTEXT_H
#define TRAP_ACTION_CONTEXT_H

class EnemyManager;
class EnemyActionContext;

class TrapActionContext {
public:
	virtual ~TrapActionContext() = default;

	virtual EnemyManager& getEnemyManager() = 0;
	virtual EnemyActionContext& getEnemyActionContext() = 0;
	virtual void onEnemyKilled() = 0;
	
	// Метод для уведомления о событии срабатывания ловушки (сущность только информирует, контекст решает, логировать ли)
	virtual void notifyTrapTriggered(int trapX, int trapY, int damage, int enemyX, int enemyY) = 0;
};

#endif

