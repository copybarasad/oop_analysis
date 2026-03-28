#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "ICombatant.h"
#include "ICooldownable.h"
#include "Health.h"
#include "Attack.h"
#include <memory>

class EnemyTower : public ICombatant, public ICooldownable {
private:
	Attack tower_attack;
	Health tower_health;
	int cooldown_remaining;
	const int max_cooldown = 2;

public:
	EnemyTower(int hp, int attack_damage);
	
	EntityType getType() const override;
	void causeDamage(int damage) override;
	int getDamage() const override;
	std::pair<int, int> getHealth() const override;
	bool alive() const override;
	std::unique_ptr<IEntity> clone() const override;
	
	bool canAttack() const override;
	void reduceCooldown() override;
	void resetCooldown() override;
	
	int getAttackRange() const;
};

#endif
