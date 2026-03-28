#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <vector>
#include <utility>
#include "FieldGrid.h"
#include "EntityManager.h"
#include "PlayerController.h"
#include "Enemy.h"
#include "EnemyTower.h"
#include "Ally.h"
#include "GameTypes.h"

class GameField {
public:
	GameField(int width, int height);
	GameField(const GameField& other);
	GameField(GameField&& other) noexcept;
	
	GameField& operator=(const GameField& other);
	GameField& operator=(GameField&& other) noexcept;
	
	~GameField();
	
	int GetWidth() const;
	int GetHeight() const;
	int GetPlayerX() const;
	int GetPlayerY() const;
	
	bool IsValidPosition(int x, int y) const;
	bool IsCellPassable(int x, int y) const;
	bool IsCellSlowing(int x, int y) const;
	
	void SetPlayerPosition(int x, int y);
	void SetCellType(int x, int y, CellType type);
	
	bool MovePlayer(int deltaX, int deltaY);
	std::vector<int> MoveEnemies();
	void UpdateBuildings();
	
	void AddEnemy(const Enemy& enemy);
	void AddEnemyBuilding(const EnemyBuilding& building);
	void AddEnemyTower(const EnemyTower& tower);
	void RemoveDeadEnemies();
	void RemoveDeadBuildings();
	
	void AttackEnemyAt(int x, int y, int damage);
	void AttackBuildingAt(int x, int y, int damage);
	void AttackArea(int startX, int startY, int areaSize, int damage);
	int GetTowerDamageAt(int x, int y) const;
	bool IsEnemyAt(int x, int y) const;
	bool IsBuildingAt(int x, int y) const;
	bool IsTowerAt(int x, int y) const;
	bool IsTrapAt(int x, int y) const;
	bool IsAllyAt(int x, int y) const;
	
	void PlaceTrap(int x, int y, int damage);
	void CheckTraps();
	void UpdateTowers();
	void AddAlly(const Ally& ally);
	std::vector<std::pair<int, int>> UpdateAllies();
	void RemoveDeadAllies();
	
	void Display() const;
	
	size_t GetEnemyCount() const;
	size_t GetBuildingCount() const;
	size_t GetTowerCount() const;
	size_t GetAllyCount() const;
	size_t GetTrapCount() const;
	
	int GetEnemyDamageAt(size_t index) const;
	int GetEnemyX(size_t index) const;
	int GetEnemyY(size_t index) const;
	bool IsEnemyAliveAt(size_t index) const;
	void AttackEnemyByIndex(size_t index, int damage);
	int GetTowerX(size_t index) const;
	int GetTowerY(size_t index) const;
	
	const Enemy& GetEnemyByIndex(size_t index) const;
	const EnemyBuilding& GetBuildingByIndex(size_t index) const;
	const EnemyTower& GetTowerByIndex(size_t index) const;
	const Ally& GetAllyByIndex(size_t index) const;
	const Trap& GetTrapByIndex(size_t index) const;

private:
	FieldGrid grid;
	EntityManager entities;
	PlayerController playerController;
	
	static const int MIN_FIELD_SIZE = 10;
	static const int MAX_FIELD_SIZE = 25;
	
	void copyFrom(const GameField& other);
	void moveFrom(GameField&& other) noexcept;
	void validateInvariant() const;
};

#endif 
