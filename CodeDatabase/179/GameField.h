#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "Cell.h"
#include "Position.h"
#include <vector>
#include <memory>
#include <iostream>

class Player;
class Enemy;
class EnemyBuilding;
class EnemyTower;
class Ally;
class Trap;

class GameField {
private:
    std::vector<std::vector<Cell>> field;
    int width;
    int height;
    
    Position playerPosition;
    std::shared_ptr<Player> player;
    std::vector<std::pair<Position, std::shared_ptr<Enemy>>> enemies;
    std::vector<std::pair<Position, std::shared_ptr<EnemyBuilding>>> buildings;
    std::vector<std::pair<Position, std::shared_ptr<EnemyTower>>> towers;
    std::vector<std::pair<Position, std::shared_ptr<Ally>>> allies;

public:
    GameField(int width, int height);
    GameField(const GameField& other);
    GameField(GameField&& other) noexcept;
    
    GameField& operator=(const GameField& other);
    GameField& operator=(GameField&& other) noexcept;
    
    int getWidth() const;
    int getHeight() const;
    
    bool isValidPosition(const Position& pos) const;
    Cell& getCell(const Position& pos);
    const Cell& getCell(const Position& pos) const;
    
    bool placePlayer(const Position& pos, std::shared_ptr<Player> playerObj);
    bool placeEnemy(const Position& pos, std::shared_ptr<Enemy> enemy);
    bool placeBuilding(const Position& pos, std::shared_ptr<EnemyBuilding> building);
    bool placeTower(const Position& pos, std::shared_ptr<EnemyTower> tower);
    bool placeAlly(const Position& pos, std::shared_ptr<Ally> ally);
    
    bool movePlayer(const Position& newPos);
    void moveEnemies();
    void moveAllies();
    void checkTraps();
    
    Position getPlayerPosition() const;
    std::shared_ptr<Player> getPlayer() const;
    
    std::vector<std::pair<Position, std::shared_ptr<Enemy>>> getEnemies() const;
    std::vector<std::pair<Position, std::shared_ptr<EnemyBuilding>>> getBuildings() const;
    std::vector<std::pair<Position, std::shared_ptr<EnemyTower>>> getTowers() const;
    std::vector<std::pair<Position, std::shared_ptr<Ally>>> getAllies() const;
    
    void removeDeadEnemies();
    std::vector<std::shared_ptr<Enemy>> getEnemiesToSpawn();
    
    bool rangedAttack(const Position& target, std::shared_ptr<Player> attacker);
    void attackPlayerFromTowers();
    
    Position getRandomAdjacentPosition(const Position& pos);
    void clearField();

    // Новые методы для сохранения/загрузки
    void save(std::ostream& file) const;
    void load(std::istream& file);
    void setPlayer(std::shared_ptr<Player> playerObj) {
            player = playerObj;
            if (player && isValidPosition(playerPosition)) {
                // Убедимся, что клетка свободна
                auto& cell = getCell(playerPosition);
                if (cell.hasObject()) {
                    cell.removeObject();
                }
                // Размещаем игрока
                cell.setObject(std::static_pointer_cast<GameObject>(player));
            }
        }
    

private:
    void initializeField();
    void deepCopy(const GameField& other);
};

#endif
