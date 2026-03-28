#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "player.h"
#include "enemy.h"
#include "game_field.h"
#include "enemy_tower.h"
#include <vector>

class GameController {
private:
    Player player;
    std::vector<Enemy> enemies;
    std::vector<EnemyTower> towers_;
    GameField field;
    bool gameRunning;
    int currentLevel_ = 1;

public:
    GameController(int fieldWidth, int fieldHeight, int enemyCount);
    GameController();

    bool movePlayer(int deltaX, int deltaY);
    bool attackEnemy(int directionX, int directionY);
    void switchPlayerCombatMode();
    void updateGame();

    bool isGameRunning() const;
    bool isGameWon() const;
    const Player& getPlayer() const;
    const std::vector<Enemy>& getEnemies() const;
    const GameField& getField() const;
    const std::vector<EnemyTower>& getTowers() const;
    std::vector<EnemyTower>& getTowers();

    void addTrap(const Trap& trap);
    void addTower(const EnemyTower& tower);
    bool usePlayerSpell(size_t index, int x, int y);

    void save(std::ostream& os) const;
    void load(std::istream& is);

    void setPlayer(Player&& p);
    void setEnemies(std::vector<Enemy>&& e);
    void setTowers(std::vector<EnemyTower>&& t);
    void setGameRunning(bool running) { gameRunning = running; }

    int getCurrentLevel() const { return currentLevel_; }
    void setCurrentLevel(int level) { currentLevel_ = level; }

    GameField& getFieldForLoad() { return field; }

    GameController(const GameController&) = delete;
    GameController& operator=(const GameController&) = delete;
    GameController(GameController&&) = default;
    GameController& operator=(GameController&&) = default;
};

#endif