#include "GameField.h"
#include <iostream>
#include <random>
#include <algorithm>
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include "TrapSpell.h"
#include "SummonSpell.h"
#include "EnhancementSpell.h"

GameField::GameField(int fieldWidth, int fieldHeight, int playerHealth, int playerMeleeDamage, int playerRangedDamage)
    : width(std::max(GameConstants::MIN_FIELD_SIZE, std::min(GameConstants::MAX_FIELD_SIZE, fieldWidth))),
      height(std::max(GameConstants::MIN_FIELD_SIZE, std::min(GameConstants::MAX_FIELD_SIZE, fieldHeight))),
      grid(height, std::vector<Cell>(width, Cell(CellType::PASSABLE))),
      player(
        std::max(1, playerHealth), 
        std::max(0, playerMeleeDamage), 
        std::max(0, playerRangedDamage)
      ),
      playerPosition(width / 2, height / 2) {
        
    if (!isValidPosition(playerPosition.x, playerPosition.y)) {
        playerPosition = Position(0, 0);
    }
    
    grid[playerPosition.y][playerPosition.x].setPlayerPresence(true);
    generateImpassableCells();
    generateSlowingCells();
}

bool GameField::isValidPosition(int x, int y) const {
    return x >= 0 && x < width && y >= 0 && y < height;
}

void GameField::generateImpassableCells() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, width * height - 1);
    int impassableCount = (width * height * GameConstants::IMPASSABLE_PERCENTAGE) / 100;

    for (int i = 0; i < impassableCount; ++i) {
        int pos = dis(gen);
        int x = pos % width;
        int y = pos / width;

        if (isValidPosition(x, y) && !(x == playerPosition.x && y == playerPosition.y)) {
            grid[y][x].setType(CellType::IMPASSABLE);
        }
    }
}

void GameField::generateSlowingCells() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, width * height - 1);

    int slowingCount = (width * height * GameConstants::SLOWING_PERCENTAGE) / 100;

    for (int i = 0; i < slowingCount; ++i) {
        int pos = dis(gen);
        int x = pos % width;
        int y = pos / width;

        if (isValidPosition(x, y) && grid[y][x].getType() == CellType::PASSABLE &&
            !(x == playerPosition.x && y == playerPosition.y)) {
            grid[y][x].setType(CellType::SLOWING);
        }
    }
}

void GameField::rebuildOccupancy() {
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			grid[y][x].setPlayerPresence(false);
			grid[y][x].setEnemyPresence(false);
			grid[y][x].setBuildingPresence(false);
			grid[y][x].setAllyPresence(false);
			grid[y][x].setTrapPresence(false);
		}
	}

	if (isValidPosition(playerPosition.x, playerPosition.y)) {
		grid[playerPosition.y][playerPosition.x].setPlayerPresence(true);
	}

	for (const auto& enemyPair : enemies) {
		if (isValidPosition(enemyPair.first.x, enemyPair.first.y)) {
			grid[enemyPair.first.y][enemyPair.first.x].setEnemyPresence(true);
		}
	}

	for (const auto& buildingPair : buildings) {
		if (isValidPosition(buildingPair.first.x, buildingPair.first.y)) {
			grid[buildingPair.first.y][buildingPair.first.x].setBuildingPresence(true);
		}
	}

	for (const auto& towerPair : towers) {
		if (isValidPosition(towerPair.first.x, towerPair.first.y)) {
			grid[towerPair.first.y][towerPair.first.x].setBuildingPresence(true);
		}
	}

	for (const auto& allyPair : allies) {
		if (isValidPosition(allyPair.first.x, allyPair.first.y)) {
			grid[allyPair.first.y][allyPair.first.x].setAllyPresence(true);
		}
	}

	for (const auto& trapPair : traps) {
		if (isValidPosition(trapPair.first.x, trapPair.first.y)) {
			grid[trapPair.first.y][trapPair.first.x].setTrapPresence(true);
		}
	}
}

void GameField::resolveTrapsInternal() {
    for (auto it = traps.begin(); it != traps.end(); ) {
        bool trapTriggered = false;
        
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
            if (enemyIt->first.x == it->first.x && enemyIt->first.y == it->first.y) {
                enemyIt->second.takeDamage(it->second);
                std::cout << "Ловушка сработала! Нанесено " << it->second << " урона врагу!" << std::endl;
                
                if (!enemyIt->second.isAlive()) {
                    player.addScore(10);
                    grid[enemyIt->first.y][enemyIt->first.x].setEnemyPresence(false);
                    enemyIt = enemies.erase(enemyIt);
                    std::cout << "Враг уничтожен!" << std::endl;
                } else {
                    ++enemyIt;
                }
                
                trapTriggered = true;
                break;
            } else {
                ++enemyIt;
            }
        }
        
        if (trapTriggered) {
            grid[it->first.y][it->first.x].setTrapPresence(false);
            it = traps.erase(it);
        } else {
            ++it;
        }
    }
}

void GameField::updateAlliesInternal() {
    std::vector<std::pair<Position, Ally>> newAllies;
    std::vector<int> directionsX = {-1, 1, 0, 0};
    std::vector<int> directionsY = {0, 0, -1, 1};
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);
    
    for (auto& allyPair : allies) {
        if (!allyPair.second.isAlive() || allyPair.second.shouldDespawn()) {
            grid[allyPair.first.y][allyPair.first.x].setAllyPresence(false);
            continue;
        }
        
        allyPair.second.updateLifetime();
        
        if (dis(gen) < 2) {
            newAllies.push_back(allyPair);
            continue;
        }
        
        int direction = dis(gen);
        int newX = allyPair.first.x + directionsX[direction];
        int newY = allyPair.first.y + directionsY[direction];
        
        if (!isValidPosition(newX, newY)) {
            newAllies.push_back(allyPair);
            continue;
        }
        
        bool attacked = false;
        for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ) {
            if (enemyIt->first.x == newX && enemyIt->first.y == newY) {
                enemyIt->second.takeDamage(allyPair.second.getDamage());
                std::cout << "Союзник атаковал врага! Нанесено " << allyPair.second.getDamage() << " урона!" << std::endl;
                
                if (!enemyIt->second.isAlive()) {
                    player.addScore(10);
                    grid[enemyIt->first.y][enemyIt->first.x].setEnemyPresence(false);
                    enemyIt = enemies.erase(enemyIt);
                    std::cout << "Враг уничтожен!" << std::endl;
                } else {
                    ++enemyIt;
                }
                
                attacked = true;
                break;
            } else {
                ++enemyIt;
            }
        }
        
        if (attacked) {
            newAllies.push_back(allyPair);
            continue;
        }
        
        if (grid[newY][newX].isPassable() && 
            !grid[newY][newX].hasPlayerOnCell() && 
            !grid[newY][newX].hasEnemyOnCell() && 
            !grid[newY][newX].hasBuildingOnCell() &&
            !grid[newY][newX].hasAllyOnCell() &&
            !grid[newY][newX].hasTrapOnCell()) {
            
            grid[allyPair.first.y][allyPair.first.x].setAllyPresence(false);
            allyPair.first = Position(newX, newY);
            grid[newY][newX].setAllyPresence(true);
        }
        
        newAllies.push_back(allyPair);
    }
    
    allies = std::move(newAllies);
}

void GameField::updateTowersInternal() {
    for (auto& towerPair : towers) {
        towerPair.second.update();
        
        if (towerPair.second.canAttackPlayer(towerPair.first.x, towerPair.first.y, 
                                           playerPosition.x, playerPosition.y)) {
            player.takeDamage(towerPair.second.getDamage());
            towerPair.second.attack();
            std::cout << "Вражеская башня атаковала вас! Нанесено " 
                      << towerPair.second.getDamage() << " урона!" << std::endl;
        }
    }
}

void GameField::switchPlayerAttackType() {
    player.switchAttackType();
}

bool GameField::attackEnemy(int dx, int dy) {
    int targetX = playerPosition.x + dx;
    int targetY = playerPosition.y + dy;
    
    if (!isValidPosition(targetX, targetY)) return false;
    
    for (auto it = enemies.begin(); it != enemies.end(); ++it) {
        if (it->first.x == targetX && it->first.y == targetY) {
            it->second.takeDamage(player.getDamage());
            std::cout << "Вы атаковали врага! Нанесено " << player.getDamage() << " урона!" << std::endl;
            
            if (!it->second.isAlive()) {
                player.addScore(10);
                grid[targetY][targetX].setEnemyPresence(false);
                enemies.erase(it);
                std::cout << "Враг уничтожен! +10 очков!" << std::endl;
            }
            return true;
        }
    }
    return false;
}

bool GameField::castSpell(int spellIndex, int targetX, int targetY) {
    if (spellIndex < 0 || spellIndex >= player.getSpellHand().getSize()) {
        std::cout << "Неверный индекс заклинания!" << std::endl;
        return false;
    }
    
    Spell* spell = player.getSpellHand().getSpell(spellIndex);
    if (!spell) {
        std::cout << "Заклинание не найдено!" << std::endl;
        return false;
    }
    
    for (int i = 0; i < player.getEnhancementStack(); i++) {
        spell->enhance();
    }
    player.resetEnhancement();
    
    return spell->cast(*this, targetX, targetY);
}

bool GameField::movePlayer(int dx, int dy) {
    if (player.getIsSlowed()) {
        player.setSlowed(false);
        return false;
    }

    int newX = playerPosition.x + dx;
    int newY = playerPosition.y + dy;

    if (!isValidPosition(newX, newY) || !grid[newY][newX].isPassable()) {
        return false;
    }

    for (const auto& enemyPair : enemies) {
        if (enemyPair.first.x == newX && enemyPair.first.y == newY) {
            player.takeDamage(enemyPair.second.getDamage());
            std::cout << "Получено " 
                      << enemyPair.second.getDamage() << " урона!" << std::endl;
            return false;
        }
    }

    grid[playerPosition.y][playerPosition.x].setPlayerPresence(false);
    playerPosition = Position(newX, newY);
    grid[playerPosition.y][playerPosition.x].setPlayerPresence(true);

    if (grid[newY][newX].getType() == CellType::SLOWING) {
        player.setSlowed(true);
    }

    return true;
}

void GameField::updateEnemies() {
    std::vector<std::pair<Position, Enemy>> newEnemies;
    std::vector<int> directionsX = {-1, 1, 0, 0};
    std::vector<int> directionsY = {0, 0, -1, 1};
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);
    
    for (auto& enemyPair : enemies) {
        if (!enemyPair.second.isAlive()) continue;
        int direction = dis(gen);
        
        int newX = enemyPair.first.x + directionsX[direction];
        int newY = enemyPair.first.y + directionsY[direction];
        
        if (!isValidPosition(newX, newY)) continue;
        
        if (newX == playerPosition.x && newY == playerPosition.y) {
            player.takeDamage(enemyPair.second.getDamage());
            std::cout << "Враг атаковал, нанесено " << enemyPair.second.getDamage() << " урона!" << std::endl;
            
            newEnemies.push_back(enemyPair);
            continue;
        }
        
        if (grid[newY][newX].isPassable() && !grid[newY][newX].hasBuildingOnCell()) {
            grid[enemyPair.first.y][enemyPair.first.x].setEnemyPresence(false);
            enemyPair.first = Position(newX, newY);
            grid[newY][newX].setEnemyPresence(true);
        }
        
        newEnemies.push_back(enemyPair);
    }
    
    enemies = std::move(newEnemies);
}

void GameField::updateBuildings() {
    for (auto& buildingPair : buildings) {
        buildingPair.second.update();
        
        if (buildingPair.second.shouldSpawnEnemy()) {
            std::vector<Position> adjacentPositions = {
                {buildingPair.first.x-1, buildingPair.first.y},
                {buildingPair.first.x+1, buildingPair.first.y},
                {buildingPair.first.x, buildingPair.first.y-1},
                {buildingPair.first.x, buildingPair.first.y+1}
            };
            
            for (const auto& pos : adjacentPositions) {
                if (isValidPosition(pos.x, pos.y) && grid[pos.y][pos.x].isPassable() && 
                    !grid[pos.y][pos.x].hasPlayerOnCell() &&
                    !grid[pos.y][pos.x].hasEnemyOnCell()) {
                    
                    addEnemy(pos.x, pos.y, buildingPair.second.getSpawnHealth(), 
                            buildingPair.second.getSpawnDamage());
                    break;
                }
            }
        }
    }
}

void GameField::updateGameState() {
    updateEnemies();
    updateBuildings();
    resolveTrapsInternal();
    updateAlliesInternal();
    updateTowersInternal();
    
    if (player.getEnemiesDefeated() >= GameConstants::ENEMY_DEFEAT_FOR_SPELL && 
        player.getSpellHand().getSize() < player.getSpellHand().getMaxSize()) {
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 4);
        
        std::unique_ptr<Spell> newSpell;
        switch (dis(gen)) {
            case 0:
                newSpell = std::make_unique<DirectDamageSpell>(15, 1);
                break;
            case 1:
                newSpell = std::make_unique<AreaDamageSpell>(10, 1);
                break;
            case 2:
                newSpell = std::make_unique<TrapSpell>(20);
                break;
            case 3:
                newSpell = std::make_unique<SummonSpell>(30, 5);
                break;
            case 4:
                newSpell = std::make_unique<EnhancementSpell>();
                break;
        }
        
        if (player.getSpellHand().addSpell(std::move(newSpell))) {
            std::cout << "Вы получили новое заклинание за победу над " 
                      << GameConstants::ENEMY_DEFEAT_FOR_SPELL << " врагами!" << std::endl;
            
            player.resetEnemiesDefeatedCounter();
        }
    }
}

void GameField::performEnemyTurn() {
	updateEnemies();
}

void GameField::performAllyTurn() {
	updateAlliesInternal();
}

void GameField::performBuildingTurn() {
	updateBuildings();
}

void GameField::performTowerTurn() {
	updateTowersInternal();
}

void GameField::resolveTraps() {
	resolveTrapsInternal();
}

void GameField::addEnemy(int x, int y, int health, int damage) {
    if (isValidPosition(x, y) && grid[y][x].isPassable() && !grid[y][x].hasEnemyOnCell()) {
        enemies.emplace_back(Position(x, y), Enemy(health, damage));
        grid[y][x].setEnemyPresence(true);
    }
}

void GameField::addBuilding(int x, int y, int spawnTurns, int enemyHealth, int enemyDamage) {
    if (isValidPosition(x, y) && grid[y][x].isPassable() && !grid[y][x].hasBuildingOnCell()) {
        buildings.emplace_back(Position(x, y), EnemyBuilding(spawnTurns, enemyHealth, enemyDamage));
        grid[y][x].setBuildingPresence(true);
    }
}

void GameField::addTower(int x, int y, int range, int damage) {
	if (isValidPosition(x, y) && grid[y][x].isPassable() && !grid[y][x].hasBuildingOnCell()) {
		towers.emplace_back(Position(x, y), EnemyTower(range, damage));
		grid[y][x].setBuildingPresence(true);
	}
}

void GameField::addAlly(int x, int y, int health, int damage) {
    if (isValidPosition(x, y) && grid[y][x].isPassable() && !grid[y][x].hasAllyOnCell()) {
        allies.emplace_back(Position(x, y), Ally(health, damage));
        grid[y][x].setAllyPresence(true);
    }
}

void GameField::addTrap(int x, int y, int damage) {
    if (isValidPosition(x, y) && grid[y][x].isPassable() && !grid[y][x].hasTrapOnCell()) {
        traps.emplace_back(Position(x, y), damage);
        grid[y][x].setTrapPresence(true);
    }
}

void GameField::display() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x == playerPosition.x && y == playerPosition.y) {
                std::cout << "P ";
            } else if (grid[y][x].hasEnemyOnCell()) {
                std::cout << "E ";
            } else if (grid[y][x].hasAllyOnCell()) {
                std::cout << "A ";
            } else if (grid[y][x].hasBuildingOnCell()) {
                std::cout << "B ";
            } else if (grid[y][x].hasTrapOnCell()) {
                std::cout << "T ";
            } else {
                switch (grid[y][x].getType()) {
                    case CellType::PASSABLE: std::cout << ". "; break;
                    case CellType::IMPASSABLE: std::cout << "# "; break;
                    case CellType::SLOWING: std::cout << "~ "; break;
                    case CellType::TRAP: std::cout << "T "; break;
                }
            }
        }
        std::cout << std::endl;
    }
    
    std::cout << "Player HP: " << player.getHealth() 
              << " | Attack: " << (player.getAttackType() == AttackType::MELEE ? "Melee" : "Ranged")
              << " | Score: " << player.getScore() 
              << " | Slowed: " << (player.getIsSlowed() ? "Yes" : "No") 
              << " | Enhancement: " << player.getEnhancementStack() << std::endl;
}

void GameField::displaySpellShop() {
    std::cout << "=== МАГАЗИН ЗАКЛИНАНИЙ ===" << std::endl;
    std::cout << "1. Прямой урон (15 урона, радиус 1) - 50 очков" << std::endl;
    std::cout << "2. Урон по площади (10 урона, 3x3) - 50 очков" << std::endl;
    std::cout << "3. Ловушка (20 урона) - 50 очков" << std::endl;
    std::cout << "4. Призыв союзника (30 HP, 5 урона) - 50 очков" << std::endl;
    std::cout << "5. Улучшение - 50 очков" << std::endl;
    std::cout << "0. Выход" << std::endl;
}

bool GameField::hasHostileUnits() const {
	return !enemies.empty() || !buildings.empty() || !towers.empty();
}

Position GameField::findRandomFreeCell() const {
	std::vector<Position> freeCells;
	freeCells.reserve(width * height);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			const Cell& cell = grid[y][x];
			if (!cell.isPassable()) {
				continue;
			}
			if (cell.hasPlayerOnCell() || cell.hasEnemyOnCell() || cell.hasBuildingOnCell()
				|| cell.hasTrapOnCell() || cell.hasAllyOnCell()) {
				continue;
			}
			freeCells.emplace_back(x, y);
		}
	}

	if (freeCells.empty()) {
		return playerPosition;
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, static_cast<int>(freeCells.size()) - 1);
	return freeCells[dis(gen)];
}

GameFieldSnapshot GameField::captureSnapshot() const {
	GameFieldSnapshot snapshot{};
	snapshot.width = width;
	snapshot.height = height;
	snapshot.playerPosition = playerPosition;
	snapshot.playerState = player.createState();

	snapshot.cellTypes.resize(height, std::vector<CellType>(width, CellType::PASSABLE));
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			snapshot.cellTypes[y][x] = grid[y][x].getType();
		}
	}

	for (const auto& enemyPair : enemies) {
		EnemySnapshot enemySnapshot{};
		enemySnapshot.position = enemyPair.first;
		enemySnapshot.health = enemyPair.second.getHealth();
		enemySnapshot.damage = enemyPair.second.getDamage();
		snapshot.enemies.push_back(enemySnapshot);
	}

	for (const auto& buildingPair : buildings) {
		BuildingSnapshot buildingSnapshot{};
		buildingSnapshot.position = buildingPair.first;
		buildingSnapshot.turnsToSpawn = buildingPair.second.getTurnsToSpawn();
		buildingSnapshot.currentTurn = buildingPair.second.getCurrentTurn();
		buildingSnapshot.spawnHealth = buildingPair.second.getSpawnHealth();
		buildingSnapshot.spawnDamage = buildingPair.second.getSpawnDamage();
		snapshot.buildings.push_back(buildingSnapshot);
	}

	for (const auto& towerPair : towers) {
		TowerSnapshot towerSnapshot{};
		towerSnapshot.position = towerPair.first;
		towerSnapshot.range = towerPair.second.getRange();
		towerSnapshot.damage = towerPair.second.getDamage();
		towerSnapshot.canAttack = towerPair.second.getCanAttackFlag();
		towerSnapshot.cooldown = towerPair.second.getCooldown();
		towerSnapshot.currentCooldown = towerPair.second.getCurrentCooldown();
		snapshot.towers.push_back(towerSnapshot);
	}

	for (const auto& allyPair : allies) {
		AllySnapshot allySnapshot{};
		allySnapshot.position = allyPair.first;
		allySnapshot.health = allyPair.second.getHealth();
		allySnapshot.damage = allyPair.second.getDamage();
		allySnapshot.lifetime = allyPair.second.getLifetime();
		snapshot.allies.push_back(allySnapshot);
	}

	for (const auto& trapPair : traps) {
		TrapSnapshot trapSnapshot{};
		trapSnapshot.position = trapPair.first;
		trapSnapshot.damage = trapPair.second;
		snapshot.traps.push_back(trapSnapshot);
	}

	return snapshot;
}

void GameField::restoreFromSnapshot(const GameFieldSnapshot& snapshot) {
	width = snapshot.width;
	height = snapshot.height;
	playerPosition = snapshot.playerPosition;

	grid.assign(height, std::vector<Cell>(width, Cell(CellType::PASSABLE)));
	for (int y = 0; y < height && y < static_cast<int>(snapshot.cellTypes.size()); ++y) {
		for (int x = 0; x < width && x < static_cast<int>(snapshot.cellTypes[y].size()); ++x) {
			grid[y][x].setType(snapshot.cellTypes[y][x]);
		}
	}

	player.applyState(snapshot.playerState);

	enemies.clear();
	for (const auto& enemySnapshot : snapshot.enemies) {
		enemies.emplace_back(enemySnapshot.position, Enemy(enemySnapshot.health, enemySnapshot.damage));
	}

	buildings.clear();
	for (const auto& buildingSnapshot : snapshot.buildings) {
		EnemyBuilding building(buildingSnapshot.turnsToSpawn, buildingSnapshot.spawnHealth, buildingSnapshot.spawnDamage);
		building.setCurrentTurn(buildingSnapshot.currentTurn);
		buildings.emplace_back(buildingSnapshot.position, building);
	}

	towers.clear();
	for (const auto& towerSnapshot : snapshot.towers) {
		EnemyTower tower(towerSnapshot.range, towerSnapshot.damage);
		tower.setState(towerSnapshot.canAttack, towerSnapshot.cooldown, towerSnapshot.currentCooldown);
		towers.emplace_back(towerSnapshot.position, tower);
	}

	allies.clear();
	for (const auto& allySnapshot : snapshot.allies) {
		Ally ally(allySnapshot.health, allySnapshot.damage);
		ally.setLifetime(allySnapshot.lifetime);
		allies.emplace_back(allySnapshot.position, ally);
	}

	traps.clear();
	for (const auto& trapSnapshot : snapshot.traps) {
		traps.emplace_back(trapSnapshot.position, trapSnapshot.damage);
	}

	rebuildOccupancy();
}

