#include "WorldManager.hpp"
#include <stdexcept>
#include <algorithm>

WorldManager::WorldManager(Field& field)
	: field(field), playerPos(0, 0), playerPlaced(false), playerSlowed(false), killCount(0), spellManager(*this) {}

bool WorldManager::isPlayerSlowed() const noexcept {
	return playerSlowed;
}

void WorldManager::addPlayer(const Coords& pos) {
	if (!field.inBounds(pos)) throw std::out_of_range("Player position out of bounds.");
	if (!field.cellAt(pos).isWalkable()) throw std::invalid_argument("Player position not walkable.");
	if (playerPlaced) {
		field.cellAt(playerPos).clearOccupant();
	}
	playerPos = pos;
	playerPlaced = true;
	field.cellAt(pos).setOccupant(OccupantType::Player, -1);
}

int WorldManager::addEnemy(const Coords& pos, const Enemy& enemyTemplate) {
	if (!field.inBounds(pos)) throw std::out_of_range("Enemy position out of bounds.");
	if (!field.cellAt(pos).isWalkable()) throw std::invalid_argument("Enemy position not walkable.");
	if (field.cellAt(pos).getOccupant() != OccupantType::None) throw std::invalid_argument("Cell already occupied.");

	int idx = static_cast<int>(enemies.size());
	enemies.emplace_back(EnemyEntry{enemyTemplate, pos});
	field.cellAt(pos).setOccupant(OccupantType::Enemy, idx);
	return idx;
}

int WorldManager::manhattan(const Coords& a, const Coords& b) const noexcept {
	return std::abs(a.x - b.x) + std::abs(a.y - b.y);
}

bool WorldManager::hasLineOfSight(const Coords& a, const Coords& b) const {
	if (a.x == b.x) {
		int y1 = std::min(a.y, b.y), y2 = std::max(a.y, b.y);
		for (int y = y1 + 1; y < y2; ++y) {
			if (!field.cellAt({a.x, y}).isWalkable()) return false;
		}
		return true;
	}
	if (a.y == b.y) {
		int x1 = std::min(a.x, b.x), x2 = std::max(a.x, b.x);
		for (int x = x1 + 1; x < x2; ++x) {
			if (!field.cellAt({x, a.y}).isWalkable()) return false;
		}
		return true;
	}
	return false;
}

MoveResult WorldManager::movePlayer(int dx, int dy, Player& player) {
	if (!playerPlaced) throw std::logic_error("Player not placed on field.");

	if (playerSlowed) {
		playerSlowed = false;
		return MoveResult::Blocked;
	}

	Coords target{ playerPos.x + dx, playerPos.y + dy };
	if (!field.inBounds(target)) return MoveResult::OutOfBounds;

	Cell& dest = field.cellAt(target);
	if (!dest.isWalkable()) return MoveResult::Blocked;

	OccupantType occ = dest.getOccupant();
	if (occ == OccupantType::None) {
		field.cellAt(playerPos).clearOccupant();
		playerPos = target;
		field.cellAt(playerPos).setOccupant(OccupantType::Player, -1);
		if (dest.getType() == CellType::Slowing) {
			playerSlowed = true;
			return MoveResult::Slowed;
		}
		return MoveResult::Moved;
	}

	if (occ == OccupantType::Enemy) {
		int enemyIndex = dest.getOccupantId();
		if (enemyIndex >= 0 && enemyIndex < static_cast<int>(enemies.size())) {
			Enemy& e = enemies[enemyIndex].enemy;
			e.takeDamage(player.currentDamage());
			if (!e.isAlive()) {
				killCount++;
				dest.clearOccupant();
				enemies.erase(enemies.begin() + enemyIndex);
				for (int i = enemyIndex; i < static_cast<int>(enemies.size()); ++i) {
					Coords pos = enemies[i].pos;
					if (field.inBounds(pos) && field.cellAt(pos).getOccupant() == OccupantType::Enemy) {
						field.cellAt(pos).setOccupant(OccupantType::Enemy, i);
					}
				}
			}
			return MoveResult::OccupiedEnemy;
		}
		return MoveResult::Blocked;
	}

	return MoveResult::Blocked;
}

void WorldManager::updateEnemyIndicesAfterRemoval(int removedIndex) {
	for (int j = removedIndex; j < static_cast<int>(enemies.size()); ++j) {
		Coords pos = enemies[j].pos;
		if (field.inBounds(pos) && field.cellAt(pos).getOccupant() == OccupantType::Enemy) {
			field.cellAt(pos).setOccupant(OccupantType::Enemy, j);
		}
	}
}

void WorldManager::handleEnemyKilled(int enemyIndex) {
	killCount++;
	Coords deadPos = enemies[enemyIndex].pos;
	field.cellAt(deadPos).clearOccupant();
	enemies.erase(enemies.begin() + enemyIndex);
	updateEnemyIndicesAfterRemoval(enemyIndex);
}

bool WorldManager::playerAttack(Player& player) {
	if (!playerPlaced) return false;
	if (enemies.empty()) return false;

	int maxRange = (player.getMode() == AttackMode::Melee) ? 1 : 4;
	
	for (int i = 0; i < static_cast<int>(enemies.size()); ++i) {
		int dist = manhattan(playerPos, enemies[i].pos);
		bool inRange = (player.getMode() == AttackMode::Melee) 
			? (dist <= maxRange)
			: (dist <= maxRange && hasLineOfSight(playerPos, enemies[i].pos));
		
		if (inRange) {
			enemies[i].enemy.takeDamage(player.currentDamage());
			if (!enemies[i].enemy.isAlive()) {
				handleEnemyKilled(i);
			}
			return true;
		}
	}
	return false;
}

void WorldManager::moveEnemies(Player& player) {
	for (int i = 0; i < static_cast<int>(enemies.size()); ++i) {
		EnemyEntry& entry = enemies[i];
		if (!entry.enemy.isAlive()) continue;

		Coords pos = entry.pos;
		Coords target = pos;

		if (playerPlaced) {
			if (playerPos.x > pos.x) target.x += 1;
			else if (playerPos.x < pos.x) target.x -= 1;

			if (playerPos.y > pos.y) target.y += 1;
			else if (playerPos.y < pos.y) target.y -= 1;
		}

		if (target == pos) {
			if (playerPlaced && manhattan(pos, playerPos) == 1) {
				player.takeDamage(2);
			}
			continue;
		}

		if (!field.inBounds(target)) continue;
		Cell& dest = field.cellAt(target);
		if (!dest.isWalkable()) continue;

		if (dest.getOccupant() == OccupantType::Player) {
			player.takeDamage(2);
			continue;
		}

		if (dest.getOccupant() == OccupantType::Enemy) continue;

		field.cellAt(pos).clearOccupant();
		entry.pos = target;
		field.cellAt(target).setOccupant(OccupantType::Enemy, i);
		
		int trapDamage = spellManager.triggerTrapAt(target);
		if (trapDamage > 0) {
			entry.enemy.takeDamage(trapDamage);
			if (!entry.enemy.isAlive()) {
				killCount++;
			}
		}
	}
	removeDeadEnemies();
	spellManager.removeInactiveTraps();
}

int WorldManager::addBuilding(const Coords& pos, const EnemyBuilding& buildingTemplate) {
	if (!field.inBounds(pos)) throw std::out_of_range("Building position out of bounds.");
	if (!field.cellAt(pos).isWalkable()) throw std::invalid_argument("Building position not walkable.");
	if (field.cellAt(pos).getOccupant() != OccupantType::None) throw std::invalid_argument("Cell already occupied.");

	int idx = static_cast<int>(buildings.size());
	buildings.emplace_back(BuildingEntry{buildingTemplate, pos});
	field.cellAt(pos).setOccupant(OccupantType::Building, idx);
	return idx;
}

void WorldManager::updateBuildings() {
	for (auto& buildingEntry : buildings) {
		if (!buildingEntry.building.isAlive()) continue;
		
		if (buildingEntry.building.update()) {
			Coords spawnPos = buildingEntry.pos;
			std::vector<Coords> possibleSpawns;
			Coords directions[4] = {{1,0}, {-1,0}, {0,1}, {0,-1}};

			for (const auto& dir : directions) {
				Coords candidate{spawnPos.x + dir.x, spawnPos.y + dir.y};
				if (field.inBounds(candidate) &&
					field.cellAt(candidate).isWalkable() &&
					field.cellAt(candidate).getOccupant() == OccupantType::None) {
					possibleSpawns.push_back(candidate);
				}
			}

			if (!possibleSpawns.empty()) {
				Coords spawnHere = possibleSpawns[0];
				addEnemy(spawnHere, Enemy(30, 5));
			}
		}
	}
}

int WorldManager::getKillCount() const noexcept {
	return killCount;
}

void WorldManager::resetKillCount() noexcept {
	killCount = 0;
}

void WorldManager::incrementKillCount() noexcept {
	killCount++;
}

void WorldManager::removeDeadEnemies() {
	for (int i = static_cast<int>(enemies.size()) - 1; i >= 0; --i) {
		if (!enemies[i].enemy.isAlive()) {
			handleEnemyKilled(i);
		}
	}
}

void WorldManager::removeDeadBuildings() {
	for (int i = static_cast<int>(buildings.size()) - 1; i >= 0; --i) {
		if (!buildings[i].building.isAlive()) {
			Coords deadPos = buildings[i].pos;
			field.cellAt(deadPos).clearOccupant();
			buildings.erase(buildings.begin() + i);
		}
	}
	for (int i = 0; i < static_cast<int>(buildings.size()); ++i) {
		Coords pos = buildings[i].pos;
		if (field.inBounds(pos)) {
			field.cellAt(pos).setOccupant(OccupantType::Building, i);
		}
	}
}


bool WorldManager::damageEnemyAt(const Coords& pos, int damage) {
	Cell& cell = field.cellAt(pos);
	OccupantType occupant = cell.getOccupant();
	if (occupant != OccupantType::Enemy) {
		return false;
	}
	
	int enemyIndex = cell.getOccupantId();
	if (enemyIndex >= 0 && enemyIndex < static_cast<int>(enemies.size())) {
		if (enemies[enemyIndex].pos == pos) {
			enemies[enemyIndex].enemy.takeDamage(damage);
			if (!enemies[enemyIndex].enemy.isAlive()) {
				killCount++;
			}
			removeDeadEnemies();
			return true;
		}
	}
	
	for (size_t i = 0; i < enemies.size(); ++i) {
		if (enemies[i].pos == pos && enemies[i].enemy.isAlive()) {
			enemies[i].enemy.takeDamage(damage);
			if (!enemies[i].enemy.isAlive()) {
				killCount++;
			}
			removeDeadEnemies();
			return true;
		}
	}
	
	return false;
}

bool WorldManager::damageBuildingAt(const Coords& pos, int damage) {
	Cell& cell = field.cellAt(pos);
	int buildingIndex = cell.getOccupantId();
	if (buildingIndex >= 0 && buildingIndex < static_cast<int>(buildings.size())) {
		buildings[buildingIndex].building.takeDamage(damage);
		removeDeadBuildings();
		return true;
	}
	return false;
}

bool WorldManager::damageTowerAt(const Coords& pos, int damage) {
	Cell& cell = field.cellAt(pos);
	int towerIndex = cell.getOccupantId();
	if (towerIndex >= 0 && towerIndex < static_cast<int>(towers.size())) {
		towers[towerIndex].tower.takeDamage(damage);
		removeDeadTowers();
		return true;
	}
	return false;
}


int WorldManager::addTower(const Coords& pos, const EnemyTower& towerTemplate) {
	if (!field.inBounds(pos)) throw std::out_of_range("Tower position out of bounds.");
	if (!field.cellAt(pos).isWalkable()) throw std::invalid_argument("Tower position not walkable.");
	if (field.cellAt(pos).getOccupant() != OccupantType::None) throw std::invalid_argument("Cell already occupied.");

	int idx = static_cast<int>(towers.size());
	towers.emplace_back(TowerEntry{towerTemplate, pos});
	field.cellAt(pos).setOccupant(OccupantType::Tower, idx);
	return idx;
}

void WorldManager::updateTowers(Player& player) {
	for (auto& towerEntry : towers) {
		if (!towerEntry.tower.isAlive()) continue;
		
		towerEntry.tower.update();
		
		if (playerPlaced && towerEntry.tower.canAttack()) {
			int dist = manhattan(towerEntry.pos, playerPos);
			if (dist <= towerEntry.tower.getRange()) {
				int damage = towerEntry.tower.attack();
				player.takeDamage(damage);
			}
		}
	}
}

void WorldManager::removeDeadTowers() {
	for (int i = static_cast<int>(towers.size()) - 1; i >= 0; --i) {
		if (!towers[i].tower.isAlive()) {
			Coords deadPos = towers[i].pos;
			field.cellAt(deadPos).clearOccupant();
			towers.erase(towers.begin() + i);
		}
	}
	for (int i = 0; i < static_cast<int>(towers.size()); ++i) {
		Coords pos = towers[i].pos;
		if (field.inBounds(pos) && field.cellAt(pos).getOccupant() == OccupantType::Tower) {
			field.cellAt(pos).setOccupant(OccupantType::Tower, i);
		}
	}
}
