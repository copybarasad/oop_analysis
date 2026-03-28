#include "LevelManager.hpp"
#include "Enums.hpp"
#include <stdexcept>
#include <random>

int LevelManager::calculateKillsRequired(int level) {
	return 5 * level;
}

void LevelManager::initializeLevel(int level, Field& field, WorldManager& world) {
	int enemyCount = 3;
	int buildingCount = level;
	int towerCount = 1;
	
	int enemiesPlaced = 0;
	std::random_device rdEnemy;
	std::mt19937 genEnemy(rdEnemy());
	std::uniform_int_distribution<> distXEnemy(0, FIELD_WIDTH - 1);
	std::uniform_int_distribution<> distYEnemy(0, FIELD_HEIGHT - 1);
	
	for (int attempt = 0; attempt < 100 && enemiesPlaced < enemyCount; ++attempt) {
		Coords pos{distXEnemy(genEnemy), distYEnemy(genEnemy)};
		if (field.inBounds(pos) && field.cellAt(pos).isWalkable() && 
			field.cellAt(pos).getOccupant() == OccupantType::None) {
			try {
				world.addEnemy(pos, Enemy(30 + level * 5, 5 + level));
				enemiesPlaced++;
			} catch (...) {
			}
		}
	}
	
	int buildingsPlaced = 0;
	std::random_device rdBuilding;
	std::mt19937 genBuilding(rdBuilding());
	std::uniform_int_distribution<> distXBuilding(0, FIELD_WIDTH - 1);
	std::uniform_int_distribution<> distYBuilding(0, FIELD_HEIGHT - 1);
	
	for (int attempt = 0; attempt < 100 && buildingsPlaced < buildingCount; ++attempt) {
		Coords pos{distXBuilding(genBuilding), distYBuilding(genBuilding)};
		if (field.inBounds(pos) && field.cellAt(pos).isWalkable() && 
			field.cellAt(pos).getOccupant() == OccupantType::None) {
			try {
				world.addBuilding(pos, EnemyBuilding(80 + level * 10, 5, -1));
				buildingsPlaced++;
			} catch (...) {
			}
		}
	}
	
	int towersPlaced = 0;
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distX(0, FIELD_WIDTH - 1);
	std::uniform_int_distribution<> distY(0, FIELD_HEIGHT - 1);
	
	while (towersPlaced < towerCount) {
		Coords pos{distX(gen), distY(gen)};
		if (field.inBounds(pos) && field.cellAt(pos).isWalkable() && 
			field.cellAt(pos).getOccupant() == OccupantType::None) {
			try {
				world.addTower(pos, EnemyTower(60 + level * 10, 10 + level * 2, 4, 2));
				towersPlaced++;
				break;
			} catch (...) {
			}
		}
	}
}

