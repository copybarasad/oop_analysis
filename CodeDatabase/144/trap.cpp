#include "trap.h"
#include "trap_action_context.h"
#include "enemymanager.h"
#include <iostream>

Trap::Trap(int x, int y, int trapDamage) 
    : positionX(x), positionY(y), damage(trapDamage), isActive(true) {}

int Trap::getPositionX() const { return positionX; }
int Trap::getPositionY() const { return positionY; }
int Trap::getDamage() const { return damage; }
bool Trap::getIsActive() const { return isActive; }

void Trap::checkAndTrigger(TrapActionContext& context) {
	if (!isActive) {
		return;
	}

	auto& enemies = context.getEnemyManager().getEnemies();
	auto& enemyActionContext = context.getEnemyActionContext();

	for (auto& enemy : enemies) {
		if (!enemy.isAlive()) {
			continue;
		}

		if (enemy.getX() == positionX && enemy.getY() == positionY) {
			enemy.takeDamage(damage);
			// Сущность только информирует о событии, не вызывая напрямую логирование
			context.notifyTrapTriggered(positionX, positionY, damage, enemy.getX(), enemy.getY());
			
			deactivate();

			if (!enemy.isAlive()) {
				enemy.onDefeated(enemyActionContext);
				enemyActionContext.rewardPlayer(10);
				context.onEnemyKilled();
			}
			break;
		}
	}
}
void Trap::setPosition(int x, int y) {
	positionX = x;
	positionY = y;
}
void Trap::setDamage(int value) {
	damage = value;
}
void Trap::setActive(bool activeState) {
	isActive = activeState;
}
void Trap::deactivate() { isActive = false; }

Trap::Snapshot Trap::createSnapshot() const {
	Snapshot snapshot;
	snapshot.positionX = positionX;
	snapshot.positionY = positionY;
	snapshot.damage = damage;
	snapshot.active = isActive;
	return snapshot;
}

Trap Trap::fromSnapshot(const Snapshot& snapshot) {
	Trap trap(snapshot.positionX, snapshot.positionY, snapshot.damage);
	trap.setActive(snapshot.active);
	return trap;
}