#include "EntityManager.h"
#include "FieldGrid.h"
#include <algorithm>
#include <random>
#include <cmath>
#include <stdexcept>

EntityManager::EntityManager() {
}

void EntityManager::AddEnemy(const Enemy& enemy) {
	enemies.push_back(enemy);
}

void EntityManager::AddEnemyBuilding(const EnemyBuilding& building) {
	buildings.push_back(building);
}

void EntityManager::AddEnemyTower(const EnemyTower& tower) {
	towers.push_back(tower);
}

void EntityManager::AddAlly(const Ally& ally) {
	if (allies.size() >= 3) {
		return;
	}
	allies.push_back(ally);
}

void EntityManager::PlaceTrap(int x, int y, int damage) {
	for (const auto& trap : traps) {
		if (trap.GetX() == x && trap.GetY() == y && !trap.IsTriggered()) {
			return;
		}
	}
	traps.emplace_back(x, y, damage);
}

void EntityManager::RemoveDeadEnemies() {
	enemies.erase(
		std::remove_if(enemies.begin(), enemies.end(),
			[](const Enemy& enemy) { return !enemy.IsAlive(); }),
		enemies.end()
	);
}

void EntityManager::RemoveDeadBuildings() {
	buildings.erase(
		std::remove_if(buildings.begin(), buildings.end(),
			[](const EnemyBuilding& building) { return !building.IsAlive(); }),
		buildings.end()
	);
}

void EntityManager::RemoveDeadAllies() {
	allies.erase(
		std::remove_if(allies.begin(), allies.end(),
			[](const Ally& ally) { return !ally.IsAlive(); }),
		allies.end()
	);
}

std::vector<int> EntityManager::MoveEnemies(FieldGrid& grid, int playerX, int playerY) {
	std::random_device randomDevice;
	std::mt19937 randomGenerator(randomDevice());
	std::uniform_int_distribution<int> directionDistribution(-1, 1);
	std::vector<int> attackingEnemies;
	
	for (size_t i = 0; i < enemies.size(); ++i) {
		Enemy& enemy = enemies[i];
		if (!enemy.IsAlive()) {
			continue;
		}
		
		int currentX = enemy.GetX();
		int currentY = enemy.GetY();
		int deltaX = directionDistribution(randomGenerator);
		int deltaY = directionDistribution(randomGenerator);
		
		int newX = currentX + deltaX;
		int newY = currentY + deltaY;
		
		if (grid.IsValidPosition(newX, newY) && grid.IsCellPassable(newX, newY)) {
			if (newX == playerX && newY == playerY) {
				attackingEnemies.push_back(static_cast<int>(i));
				continue;
			}
			if (IsEntityAt(newX, newY)) {
				continue;
			}
			enemy.SetPosition(newX, newY);
			
			if (IsTrapAt(newX, newY)) {
				for (auto& trap : traps) {
					if (trap.GetX() == newX && trap.GetY() == newY && !trap.IsTriggered()) {
						enemy.TakeDamage(trap.GetDamage());
						trap.Trigger();
						break;
					}
				}
			}
		}
	}
	
	return attackingEnemies;
}

void EntityManager::UpdateBuildings(FieldGrid& grid, int playerX, int playerY) {
	for (auto& building : buildings) {
		building.Update();
		if (building.ShouldSpawnEnemy()) {
			const int bx = building.GetX();
			const int by = building.GetY();
			bool spawned = false;
			for (int dy = -1; dy <= 1 && !spawned; ++dy) {
				for (int dx = -1; dx <= 1 && !spawned; ++dx) {
					if (dx == 0 && dy == 0) continue;
					int sx = bx + dx;
					int sy = by + dy;
					if (!grid.IsValidPosition(sx, sy)) continue;
					if (!grid.IsCellPassable(sx, sy)) continue;
					if (IsEntityAt(sx, sy)) continue;
					if (sx == playerX && sy == playerY) continue;
					Enemy newEnemy(50, 10);
					newEnemy.SetPosition(sx, sy);
					enemies.push_back(newEnemy);
					spawned = true;
				}
			}
			building.ResetSpawnCounter();
		}
	}
}

void EntityManager::CheckTraps(FieldGrid& grid) {
	for (auto& trap : traps) {
		if (trap.IsTriggered()) {
			continue;
		}
		if (IsEnemyAt(trap.GetX(), trap.GetY())) {
			AttackEnemyAt(trap.GetX(), trap.GetY(), trap.GetDamage());
			trap.Trigger();
		}
	}
	
	traps.erase(
		std::remove_if(traps.begin(), traps.end(),
			[](const Trap& trap) { return trap.IsTriggered(); }),
		traps.end()
	);
}


void EntityManager::AttackEnemyAt(int x, int y, int damage) {
	for (auto& enemy : enemies) {
		if (enemy.GetX() == x && enemy.GetY() == y && enemy.IsAlive()) {
			enemy.TakeDamage(damage);
			break;
		}
	}
}

void EntityManager::AttackBuildingAt(int x, int y, int damage) {
	for (auto& building : buildings) {
		if (building.GetX() == x && building.GetY() == y && building.IsAlive()) {
			building.TakeDamage(damage);
			break;
		}
	}
}

void EntityManager::AttackArea(int startX, int startY, int areaSize, int damage) {
	for (int y = startY; y < startY + areaSize; ++y) {
		if (y < 0) continue;
		for (int x = startX; x < startX + areaSize; ++x) {
			if (x < 0) continue;
			if (IsEnemyAt(x, y)) {
				AttackEnemyAt(x, y, damage);
			}
			if (IsBuildingAt(x, y)) {
				AttackBuildingAt(x, y, damage);
			}
		}
	}
}

bool EntityManager::IsEnemyAt(int x, int y) const {
	for (const auto& enemy : enemies) {
		if (enemy.GetX() == x && enemy.GetY() == y && enemy.IsAlive()) {
			return true;
		}
	}
	return false;
}

bool EntityManager::IsBuildingAt(int x, int y) const {
	for (const auto& building : buildings) {
		if (building.GetX() == x && building.GetY() == y) {
			return true;
		}
	}
	return false;
}

bool EntityManager::IsTowerAt(int x, int y) const {
	for (const auto& tower : towers) {
		if (tower.GetX() == x && tower.GetY() == y) {
			return true;
		}
	}
	return false;
}

bool EntityManager::IsTrapAt(int x, int y) const {
	for (const auto& trap : traps) {
		if (trap.GetX() == x && trap.GetY() == y && !trap.IsTriggered()) {
			return true;
		}
	}
	return false;
}

bool EntityManager::IsAllyAt(int x, int y) const {
	for (const auto& ally : allies) {
		if (ally.GetX() == x && ally.GetY() == y && ally.IsAlive()) {
			return true;
		}
	}
	return false;
}

bool EntityManager::IsEntityAt(int x, int y) const {
	return IsEnemyAt(x, y) || IsBuildingAt(x, y) || IsTowerAt(x, y) || 
		   IsAllyAt(x, y) || IsTrapAt(x, y);
}

size_t EntityManager::GetEnemyCount() const {
	return enemies.size();
}

size_t EntityManager::GetBuildingCount() const {
	return buildings.size();
}

size_t EntityManager::GetTowerCount() const {
	return towers.size();
}

const Enemy& EntityManager::GetEnemyAt(size_t index) const {
	if (index >= enemies.size()) {
		throw std::out_of_range("Enemy index out of range");
	}
	return enemies[index];
}

Enemy& EntityManager::GetEnemyAt(size_t index) {
	if (index >= enemies.size()) {
		throw std::out_of_range("Enemy index out of range");
	}
	return enemies[index];
}

const EnemyTower& EntityManager::GetTowerAt(size_t index) const {
	if (index >= towers.size()) {
		throw std::out_of_range("Tower index out of range");
	}
	return towers[index];
}

EnemyTower& EntityManager::GetTowerAt(size_t index) {
	if (index >= towers.size()) {
		throw std::out_of_range("Tower index out of range");
	}
	return towers[index];
}

size_t EntityManager::GetAllyCount() const {
	return allies.size();
}

const Ally& EntityManager::GetAllyAt(size_t index) const {
	if (index >= allies.size()) {
		throw std::out_of_range("Ally index out of range");
	}
	return allies[index];
}

Ally& EntityManager::GetAllyAt(size_t index) {
	if (index >= allies.size()) {
		throw std::out_of_range("Ally index out of range");
	}
	return allies[index];
}

const EnemyBuilding& EntityManager::GetBuildingAt(size_t index) const {
	if (index >= buildings.size()) {
		throw std::out_of_range("Building index out of range");
	}
	return buildings[index];
}

EnemyBuilding& EntityManager::GetBuildingAt(size_t index) {
	if (index >= buildings.size()) {
		throw std::out_of_range("Building index out of range");
	}
	return buildings[index];
}

const Trap& EntityManager::GetTrapAt(size_t index) const {
	if (index >= traps.size()) {
		throw std::out_of_range("Trap index out of range");
	}
	return traps[index];
}

Trap& EntityManager::GetTrapAt(size_t index) {
	if (index >= traps.size()) {
		throw std::out_of_range("Trap index out of range");
	}
	return traps[index];
}

size_t EntityManager::GetTrapCount() const {
	return traps.size();
}

