#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "enemybuilding.h"
#include "tower_action_context.h"

class EnemyTower : public EnemyBuilding {
private:
    int attackRange;
    int damage;
    bool canAttack;
    int attackCooldown;
    int currentCooldown;

public:
	struct Snapshot {
		EnemyBuilding::Snapshot buildingSnapshot;
		int attackRange = 0;
		int damage = 0;
		bool canAttack = true;
		int attackCooldown = 0;
		int currentCooldown = 0;
		bool readyToAttack = false;
	};

    EnemyTower(int spawnInterval = 5, int x = -1, int y = -1, int range = 4, int dmg = 8, int health = 150);
    
    void update() override;
    void attackPlayer(TowerActionContext& context);
    bool canAttackPlayer(const TowerActionContext& context) const;
    int getAttackRange() const;
    int getDamage() const;
    int getAttackCooldown() const;
    int getCurrentCooldown() const;
    bool isReadyToAttack() const;
    void setAttackRange(int range);
    void setDamage(int value);
    void setAttackCooldown(int cooldown);
    void setCurrentCooldown(int cooldown);
    void setReadyToAttack(bool ready);
	Snapshot createSnapshot() const;
	static EnemyTower fromSnapshot(const Snapshot& snapshot);
};

#endif