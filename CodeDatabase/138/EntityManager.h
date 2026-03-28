#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>
#include <utility>
#include "Enemy.h"
#include "EnemyBuilding.h"
#include "EnemyTower.h"
#include "Trap.h"
#include "Ally.h"

class FieldGrid;

class EntityManager {
public:
	EntityManager();
	
	void AddEnemy(const Enemy& enemy);
	void AddEnemyBuilding(const EnemyBuilding& building);
	void AddEnemyTower(const EnemyTower& tower);
	void AddAlly(const Ally& ally);
	void PlaceTrap(int x, int y, int damage);
	
	void RemoveDeadEnemies();
	void RemoveDeadBuildings();
	void RemoveDeadAllies();
	
	std::vector<int> MoveEnemies(FieldGrid& grid, int playerX, int playerY);
	void UpdateBuildings(FieldGrid& grid, int playerX, int playerY);
	void CheckTraps(FieldGrid& grid);
	
	void AttackEnemyAt(int x, int y, int damage);
	void AttackBuildingAt(int x, int y, int damage);
	void AttackArea(int startX, int startY, int areaSize, int damage);
	
	bool IsEnemyAt(int x, int y) const;
	bool IsBuildingAt(int x, int y) const;
	bool IsTowerAt(int x, int y) const;
	bool IsTrapAt(int x, int y) const;
	bool IsAllyAt(int x, int y) const;
	bool IsEntityAt(int x, int y) const;
	
	size_t GetEnemyCount() const;
	size_t GetBuildingCount() const;
	size_t GetTowerCount() const;
	size_t GetAllyCount() const;
	
	const Enemy& GetEnemyAt(size_t index) const;
	Enemy& GetEnemyAt(size_t index);
	const EnemyBuilding& GetBuildingAt(size_t index) const;
	EnemyBuilding& GetBuildingAt(size_t index);
	const EnemyTower& GetTowerAt(size_t index) const;
	EnemyTower& GetTowerAt(size_t index);
	const Ally& GetAllyAt(size_t index) const;
	Ally& GetAllyAt(size_t index);
	const Trap& GetTrapAt(size_t index) const;
	Trap& GetTrapAt(size_t index);
	size_t GetTrapCount() const;

private:
	std::vector<Enemy> enemies;
	std::vector<EnemyBuilding> buildings;
	std::vector<EnemyTower> towers;
	std::vector<Trap> traps;
	std::vector<Ally> allies;
};

#endif

