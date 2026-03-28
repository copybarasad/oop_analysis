#include "enemytower.h"
#include <cmath>

EnemyTower::EnemyTower(int spawnInterval, int x, int y, int range, int dmg, int health)
    : EnemyBuilding(spawnInterval, x, y, health), attackRange(range), damage(dmg), 
      canAttack(true), attackCooldown(2), currentCooldown(0) {}

void EnemyTower::update() {
    EnemyBuilding::update();
    
    if (currentCooldown > 0) {
        currentCooldown--;
        canAttack = false;
    } else {
        canAttack = true;
    }
}

bool EnemyTower::canAttackPlayer(const TowerActionContext& context) const {
    if (isDestroyed()) return false;
    
    int playerX = context.getPlayerX();
    int playerY = context.getPlayerY();
    int dx = std::abs(playerX - getX());
    int dy = std::abs(playerY - getY());
    return (dx <= attackRange && dy <= attackRange && 
            context.hasLineOfSight(getX(), getY(), playerX, playerY));
}

void EnemyTower::attackPlayer(TowerActionContext& context) {
    if (!canAttack || isDestroyed()) return;
    
    if (canAttackPlayer(context)) {
        int weakenedDamage = damage / 2;
        int playerHealth = context.applyDamageToPlayer(weakenedDamage);
        
        // Сущность только информирует о событии, не вызывая напрямую логирование
        context.notifyTowerAttack(getX(), getY(), weakenedDamage, playerHealth);
        
        currentCooldown = attackCooldown;
        canAttack = false;
    }
}

int EnemyTower::getAttackRange() const {
    return attackRange;
}

int EnemyTower::getDamage() const {
    return damage;
}

int EnemyTower::getAttackCooldown() const {
	return attackCooldown;
}

int EnemyTower::getCurrentCooldown() const {
	return currentCooldown;
}

bool EnemyTower::isReadyToAttack() const {
	return canAttack;
}

void EnemyTower::setAttackRange(int range) {
	attackRange = range;
}

void EnemyTower::setDamage(int value) {
	damage = value;
}

void EnemyTower::setAttackCooldown(int cooldown) {
	attackCooldown = cooldown;
}

void EnemyTower::setCurrentCooldown(int cooldown) {
	currentCooldown = cooldown;
}

void EnemyTower::setReadyToAttack(bool ready) {
	canAttack = ready;
}

EnemyTower::Snapshot EnemyTower::createSnapshot() const {
	Snapshot snapshot;
	snapshot.buildingSnapshot = EnemyBuilding::createSnapshot();
	snapshot.attackRange = attackRange;
	snapshot.damage = damage;
	snapshot.canAttack = canAttack;
	snapshot.attackCooldown = attackCooldown;
	snapshot.currentCooldown = currentCooldown;
	snapshot.readyToAttack = isReadyToAttack();
	return snapshot;
}

EnemyTower EnemyTower::fromSnapshot(const Snapshot& snapshot) {
	auto base = snapshot.buildingSnapshot;
	EnemyTower tower(base.spawnInterval, base.positionX, base.positionY, snapshot.attackRange, snapshot.damage, base.maxHealth);
	tower.setTurnsUntilSpawn(base.turnsUntilSpawn);
	tower.setHealth(base.health);
	tower.setMaxHealth(base.maxHealth);
	tower.setAttackCooldown(snapshot.attackCooldown);
	tower.setCurrentCooldown(snapshot.currentCooldown);
	tower.setReadyToAttack(snapshot.readyToAttack);
	tower.setDamage(snapshot.damage);
	return tower;
}