#pragma once
#include "Entities/Player.h"
#include "Entities/Enemy.h"
#include "Entities/Ally.h"
#include "Entities/Field.h"
#include "Entities/Trap.h"
#include "Entities/ArcherTower.h"
#include "Entities/Tower.h"
#include "ISpellContext.h"
#include "JsonLib/json.hpp"


class GameBoard : public ISpellContext
{
private:
	Player* player = nullptr;
	Field* field = nullptr;
	ArcherTower* archerTower = nullptr;
	Tower* tower = nullptr;
	std::vector<Ally> allies;
	std::vector<Enemy> enemies;
	std::vector<Trap> traps;
public:
	GameBoard() = default;
	GameBoard(nlohmann::json& js);
	void createField(int width, int height, int cellSize);
	void createField(const Field& field);
	void createPlayer(unsigned int health, unsigned int damage);
	void createPlayer(unsigned int health, unsigned int damage, Point& pos);
	void createPlayer(const Player& player);
	//void createPlayer(unsigned int health, unsigned int damage, unsigned int speed, unsigned short longFightDistance, std::string name);
	void spawnArcherTower(const Point& pos);
	void initializeField();
	void spawnEnemy(unsigned int health, unsigned int damage);
	void spawnTower(unsigned int health, unsigned int radius, unsigned int spawnTime);
	void spawnAlly(int health, int damage, int alliesCount, const Point& point) override;
	std::vector<Enemy*> getEnemiesAt(const Point& point, int radius) override;
	Entity* getEntityAt(const Point& point) override;
	Enemy* getEnemyAt(const Point& point) override;
	Tower* getTowerAt(const Point& point, int radius) override;
	Tower* getTowerAt(const Point& point) override;
	bool spawnTrap(const Point& point) override;
	void upgradeSpell(int count) override;
	const Enemy* getEnemy(const Point& point) const;
	const Entity* getEntityById(const int id) const;
	const std::vector<Ally>& getAllies() const;
	std::vector<Ally>& getAllies();
	const std::vector<Enemy>& getEnemies() const;
	std::vector<Enemy>& getEnemies();
	const std::vector<Trap>& getTraps() const;
	std::vector<Trap>& getTraps();
	Trap* getTrap(const Point& point);
	Field* getField();
	const Field* getField() const;
	Player* getPlayer();
	const Player* getPlayer() const;
	const ArcherTower* getArcherTower() const;
	ArcherTower* getArcherTower();
	Tower* getTower();
	const Tower* getTower() const;
	~GameBoard();
private:
	void spawnPlayer();
};