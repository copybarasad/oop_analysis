#pragma once
#include "enemy.h"
#include "enemy_tower.h" 
#include <vector>

class Player;
class GameField;
enum class CellType; 

class EnemyManager {
public:
	void AddEnemy(const Enemy& e);
	void InitializeEnemies(const GameField& field, int count, int base_hp = 30, int base_damage = 12);
	
	void AddTower(const Position& pos, GameField& field);
    void UpdateTowers(Player& player, GameField& field);

	std::vector<Enemy>& GetEnemies();
	const std::vector<Enemy>& GetEnemies() const;
	std::vector<EnemyTower>& GetTowers();
	const std::vector<EnemyTower>& GetTowers() const;

	void MoveEnemies(Player& player, GameField& field);
	bool AnyAlive() const;

	void Clear();

private:
	std::vector<Enemy> enemies_;
	std::vector<EnemyTower> towers_;
};
