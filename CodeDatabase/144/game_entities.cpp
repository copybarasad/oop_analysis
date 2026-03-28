#include "game.h"
#include "ally.h"
#include "trap.h"
#include "damageable.h"
#include "game_enemy_action_context.h"
#include "enemy.h"
#include "enemybuilding.h"
#include "enemytower.h"
#include <algorithm>
#include <iostream>

bool Game::isCellOccupiedByAlly(int x, int y) const {
	for (const auto& ally : allies) {
		if (ally.isAlive() && ally.getPositionX() == x && ally.getPositionY() == y) {
			return true;
		}
	}
	return false;
}

bool Game::isCellOccupiedByEnemy(int x, int y) const {
	return enemyManager.isCellOccupiedByEnemy(x, y);
}

bool Game::damageEntitiesAtPosition(int x, int y, int damageAmount) {
	bool hitTarget = false;
	GameEnemyActionContext defeatContext(*this);
	
	for (auto& enemy : enemyManager.getEnemies()) {
		if (enemy.isAlive() && enemy.getPositionX() == x && enemy.getPositionY() == y) {
			enemy.takeDamage(damageAmount);
			if (!enemy.isAlive()) {
				enemy.onDefeated(defeatContext);
			}
			hitTarget = true;
		}
	}
	
	for (auto& building : buildingManager.getBuildings()) {
		if (building.isAlive() && building.getPositionX() == x && building.getPositionY() == y) {
			building.takeDamage(damageAmount);
			if (!building.isAlive()) {
				building.onDefeated(defeatContext);
			}
			hitTarget = true;
		}
	}
	
	for (auto& tower : buildingManager.getTowers()) {
		if (tower.isAlive() && tower.getPositionX() == x && tower.getPositionY() == y) {
			tower.takeDamage(damageAmount);
			if (!tower.isAlive()) {
				tower.onDefeated(defeatContext);
			}
			hitTarget = true;
		}
	}
	
	return hitTarget;
}

void Game::addAlly(const Ally& ally) {
	allies.push_back(ally);
}

void Game::addTrap(const Trap& trap) {
	traps.push_back(trap);
}

void Game::removeTrap(int index) {
	if (index >= 0 && index < traps.size()) {
		traps.erase(traps.begin() + index);
	}
}