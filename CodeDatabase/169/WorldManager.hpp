#pragma once
#include <vector>
#include "Field.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "EnemyBuilding.hpp"
#include "EnemyTower.hpp"
#include "SpellManager.hpp"

class WorldManager {
public:
	struct EnemyEntry {
		Enemy enemy;
		Coords pos;
	};
	struct BuildingEntry {
		EnemyBuilding building;
		Coords pos;
	};
	struct TowerEntry {
		EnemyTower tower;
		Coords pos;
	};
	
	explicit WorldManager(Field& field);

	void addPlayer(const Coords& pos);
	MoveResult movePlayer(int dx, int dy, Player& player);
	bool playerAttack(Player& player);

	int addEnemy(const Coords& pos, const Enemy& enemyTemplate);
	void moveEnemies(Player& player);

	int addBuilding(const Coords& pos, const EnemyBuilding& buildingTemplate);
	void updateBuildings();
	
	int getKillCount() const noexcept;
	void resetKillCount() noexcept;
	void incrementKillCount() noexcept;
	
	int addTower(const Coords& pos, const EnemyTower& towerTemplate);
	void updateTowers(Player& player);

	int manhattan(const Coords& a, const Coords& b) const noexcept;
	bool hasLineOfSight(const Coords& a, const Coords& b) const;
	bool isPlayerSlowed() const noexcept;
	const Coords& getPlayerPos() const noexcept { return playerPos; }
	
	Field& getField() noexcept { return field; }
	const Field& getField() const noexcept { return field; }
	
	bool damageEnemyAt(const Coords& pos, int damage);
	bool damageBuildingAt(const Coords& pos, int damage);
	bool damageTowerAt(const Coords& pos, int damage);
	
	SpellManager& getSpellManager() noexcept { return spellManager; }
	const SpellManager& getSpellManager() const noexcept { return spellManager; }
	
	void setKillCount(int count) noexcept { killCount = count; }
	size_t getEnemyCount() const noexcept { return enemies.size(); }
	size_t getBuildingCount() const noexcept { return buildings.size(); }
	size_t getTowerCount() const noexcept { return towers.size(); }
	
	const std::vector<EnemyEntry>& getEnemies() const { return enemies; }
	const std::vector<BuildingEntry>& getBuildings() const { return buildings; }
	const std::vector<TowerEntry>& getTowers() const { return towers; }
	
	bool isPlayerPlaced() const noexcept { return playerPlaced; }
	void setPlayerSlowed(bool slowed) noexcept { playerSlowed = slowed; }

private:
	Field& field;
	Coords playerPos;
	bool playerPlaced;
	bool playerSlowed;

	std::vector<EnemyEntry> enemies;
	std::vector<BuildingEntry> buildings;
	std::vector<TowerEntry> towers;
	int killCount;
	
	SpellManager spellManager;
	
	void removeDeadEnemies();
	void removeDeadBuildings();
	void removeDeadTowers();
	void updateEnemyIndicesAfterRemoval(int removedIndex);
	void handleEnemyKilled(int enemyIndex);
};
