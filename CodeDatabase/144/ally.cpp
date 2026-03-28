#include "ally.h"
#include "ally_action_context.h"
#include "enemymanager.h"
#include "field.h"
#include <cmath>
#include <iostream>
#include <algorithm>

Ally::Ally(int health, int damage, int x, int y) 
    : health(health), damage(damage), positionX(x), positionY(y), movementCooldown(0) {}

void Ally::update(AllyActionContext& context) {
    if (!isAlive()) return;
    
    if (movementCooldown > 0) {
        movementCooldown--;
    }
    
    attackEnemyInRange(context);
    
    if (movementCooldown == 0) {
        moveTowardsNearestEnemy(context);
        movementCooldown = 2;
    }
}

void Ally::moveTowardsNearestEnemy(AllyActionContext& context) {
    auto& enemies = context.getEnemyManager().getEnemies();
    auto& field = context.getField();
    
    int nearestX = -1, nearestY = -1;
    double minDistance = 1000.0;
    for (const auto& enemy : enemies) {
        if (enemy.isAlive()) {
            double dist = std::sqrt(std::pow(enemy.getX() - positionX, 2) + 
                                   std::pow(enemy.getY() - positionY, 2));
            if (dist < minDistance) {
                minDistance = dist;
                nearestX = enemy.getX();
                nearestY = enemy.getY();
            }
        }
    }
    
    if (nearestX != -1 && nearestY != -1) {
        int newX = positionX, newY = positionY;
        field.findPathToPlayer(positionX, positionY, nearestX, nearestY, newX, newY);
        if ((newX != positionX || newY != positionY) &&
            field.isCellPassable(newX, newY) &&
            !context.isCellOccupiedByEnemy(newX, newY) &&
            !context.isCellOccupiedByBuilding(newX, newY) &&
            !context.isCellOccupiedByAlly(newX, newY)) {
            positionX = newX;
            positionY = newY;
        }
    }
}

void Ally::attackEnemyInRange(AllyActionContext& context) {
    auto& enemies = context.getEnemyManager().getEnemies();
    
    for (auto& enemy : enemies) {
        if (enemy.isAlive()) {
            int dx = std::abs(enemy.getX() - positionX);
            int dy = std::abs(enemy.getY() - positionY);
            if (dx <= 1 && dy <= 1) {
                enemy.takeDamage(damage);
                // Сущность только информирует о событии, не вызывая напрямую логирование
                context.notifyAllyAttack(positionX, positionY, damage, enemy.getX(), enemy.getY());
                if (!enemy.isAlive()) {
                    context.rewardPlayerForEnemyKill(5);
                }
                break;
            }
        }
    }
}

void Ally::takeDamage(int damageAmount) {
    health = std::max(0, health - damageAmount);
}

int Ally::getPositionX() const { return positionX; }
int Ally::getPositionY() const { return positionY; }
int Ally::getHealth() const { return health; }
int Ally::getDamage() const { return damage; }
int Ally::getMovementCooldown() const { return movementCooldown; }
bool Ally::isAlive() const { return health > 0; }

void Ally::setPosition(int x, int y) {
	positionX = x;
	positionY = y;
}

void Ally::setHealth(int value) {
	health = value;
}

void Ally::setDamage(int value) {
	damage = value;
}

void Ally::setMovementCooldown(int cooldown) {
	movementCooldown = cooldown;
}

Ally::Snapshot Ally::createSnapshot() const {
	Snapshot snapshot;
	snapshot.health = health;
	snapshot.damage = damage;
	snapshot.positionX = positionX;
	snapshot.positionY = positionY;
	snapshot.movementCooldown = movementCooldown;
	snapshot.alive = isAlive();
	return snapshot;
}

Ally Ally::fromSnapshot(const Snapshot& snapshot) {
	Ally ally(snapshot.health, snapshot.damage, snapshot.positionX, snapshot.positionY);
	ally.setHealth(snapshot.health);
	ally.setDamage(snapshot.damage);
	ally.setMovementCooldown(snapshot.movementCooldown);
	if (!snapshot.alive) {
		ally.setHealth(0);
	}
	return ally;
}
