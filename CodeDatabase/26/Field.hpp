#pragma once
#include <iosfwd>
#include <vector>
#include <optional>
#include <ostream>
#include <random>
#include "Position.hpp"
#include "Cell.hpp"
#include "Player.hpp"
#include "Enemy.hpp"
#include "EnemyNest.hpp"
#include "Direction.hpp"

class EnemyController;
class NestController;

class Field {
public:
	Field(int width, int height,
	      unsigned int seed = 0,
	      double wallProbability = 0.12,
	      double slowProbability = 0.10);

	Field(const Field& other);
	Field(Field&& other) noexcept;
	Field& operator=(const Field& other);
	Field& operator=(Field&& other) noexcept;
	~Field();

	int width() const noexcept;
	int height() const noexcept;

	void placePlayer(const Player& player, const Position& pos);
	void addEnemy(const Enemy& enemy, const Position& pos);
	void addNest(const EnemyNest& nest, const Position& pos);

	void placePlayerRandom(const Player& player);
	void addEnemyRandom(const Enemy& enemy);
	void addNestRandom(const EnemyNest& nest);

	void setEnemySpawnStats(int hp, int dmg);

	void advanceTurn();

	bool isGameOver() const noexcept;
	Player& player() noexcept;
	const Player& player() const noexcept;

	char symbolAt(const Position& p) const noexcept;
	int enemyCount() const noexcept;
	int lastPlayerDamage() const noexcept;
	bool isVictory() const noexcept;
	int enemySpawnHp() const noexcept;
	int enemySpawnDamage() const noexcept;

	bool tryPlacePlayerRandom(const Player& player);
	bool tryAddEnemyRandom(const Enemy& enemy);
	bool tryAddNestRandom(const EnemyNest& nest);

	std::optional<Position> playerPosition() const noexcept;

	bool contains(const Position& p) const noexcept;
	bool isCellPassable(const Position& p) const noexcept;
	bool isCellSlow(const Position& p) const noexcept;

	bool hasEnemyAt(const Position& p) const noexcept;
	bool hasNestAt(const Position& p) const noexcept;

	bool hasAllyAt(const Position& p) const noexcept;

	bool hasTrapAt(const Position& p) const noexcept;
	bool hasTowerAt(const Position& p) const noexcept;

	void recordAllyDamageRequest(const Position& p, int damage);
	std::vector<std::pair<Position,int>> drainAllyDamageRequests();

	bool isCellFreeForStructure(const Position& p) const noexcept;

	bool dealDamageToEnemyAt(const Position& p, int damage);
	bool dealDamageToNestAt(const Position& p, int damage);
	int enemyDamageAt(const Position& p) const noexcept;
	bool removeNestAt(const Position& p);

	bool registerTowerAt(const Position& p);
	void removeTowerAt(const Position& p);

	std::vector<Position> enemyPositions() const;

	bool applyDamageToPlayer(int damage);
	void setAllyOverlay(const std::vector<Position>& allies);
	void setTrapOverlay(const std::vector<Position>& traps);
	void save(std::ostream& out) const;
	static Field load(std::istream& in);

private:
    struct EnemyEntry {
        Enemy enemy;
        Position pos;
		bool skipMoveNextTurn{false};
    };

    struct NestEntry {
        EnemyNest nest;
        Position pos;
        int counter;
        int health;
    };

    int width_;
    int height_;
    std::vector<std::vector<Cell>> grid_;
    std::optional<Position> playerPos_;
    Player player_;
    std::vector<EnemyEntry> enemies_;
    std::vector<NestEntry> nests_;
	std::vector<Position> towerPositions_;
	std::vector<Position> allyOverlay_;
	std::vector<Position> trapOverlay_;
    std::vector<std::pair<Position,int>> pendingAllyDamage_;

    unsigned int seed_;
    std::mt19937 rng_;
    double wallProb_;
    double slowProb_;

    int enemySpawnHp_{5};
    int enemySpawnDmg_{2};
    int lastPlayerDamage_{0};

    bool isInside(const Position& p) const noexcept;
    bool cellIsFreeAndPassable(const Position& p) const noexcept;
    bool cellIsPassable(const Position& p) const noexcept;
    int  findEnemyAt(const Position& p) const noexcept;
    int  findNestAt(const Position& p) const noexcept;
    Cell& at(const Position& p);
    const Cell& at(const Position& p) const;
    Position stepOf(Direction dir, const Position& from) const noexcept;
    void generateObstacles_();
    Position randomFreeCell_();
    void applyDamageToEnemy_(int index, int damage);
	Position randomFreeNonSlowCell_();

    friend class EnemyController;
	friend class NestController;
	friend class Player;
	friend class GameWorld;
};




