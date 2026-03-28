#ifndef ENEMY_ACTION_CONTEXT_H
#define ENEMY_ACTION_CONTEXT_H

class EnemyActionContext {
public:
	virtual ~EnemyActionContext() = default;

	virtual void rewardPlayer(int points) = 0;
};

#endif


