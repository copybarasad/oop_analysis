#ifndef GAME_BOARD_H
#define GAME_BOARD_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

#include "cell.hpp"
#include "player.hpp"
#include "enemy.hpp"
#include "EnemyBuilding.hpp"
#include "EnemyTower.hpp"
#include "ally.hpp"
#include "trap.hpp"
#include "spell.hpp"
#include "observer.hpp" 

class GameBoard : public IObservable {
private:
    int width;
    int height;
    Cell** grid;
    
    Player* player;
    
    std::vector<Enemy*> enemies;
    std::vector<EnemyBuilding*> buildings;
    std::vector<EnemyTower*> towers;
    std::vector<Ally*> allies;
    std::vector<Trap*> traps;

    std::vector<std::string> message_log;

    void allocateMemory();
    void freeMemory();
    void copyFrom(const GameBoard& other);

    bool isDiagonalPathBlocked(int x1, int y1, int x2, int y2);

    template<typename T>
    void removeEntityFromVector(std::vector<T*>& vec, T* entity) {
        auto it = std::find(vec.begin(), vec.end(), entity);
        if (it != vec.end()) {
            this->grid[(*it)->getY()][(*it)->getX()].setEntity(nullptr);
            delete *it;
            vec.erase(it);
        }
    }

public:
    GameBoard(int w, int h);
    ~GameBoard();
    GameBoard(const GameBoard& other);
    GameBoard& operator=(const GameBoard& other);
    GameBoard(GameBoard&& other) noexcept;
    GameBoard& operator=(GameBoard&& other) noexcept;

    void initializeBoard(double wall_chance, double slow_chance);

    void addPlayer(Player* p);
    void addEnemy(Enemy* e);
    void addBuilding(EnemyBuilding* b);
    void addTower(EnemyTower* t);
    void addAlly(Ally* a);
    bool addTrap(Trap* t);

    bool movePlayer(int dx, int dy);
    bool playerAttack();
    bool playerCastSpell(int spell_index, int targetX, int targetY);

    void moveEnemies();
    void updateBuildings();
    void updateTowers();
    void moveAllies();
    void enemyAttack(Enemy* enemy);
    void damageArea(int cx, int cy, int size, int damage);

    void addLogMessage(const std::string& message);
    void clearLog();

    Player* getPlayer() const { return player; }
    Entity* getEntityAt(int x, int y) const;
    
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    
    bool isGround(int x, int y) const;
    bool isWall(int x, int y) const { 
        if(x < 0 || x >= width || y < 0 || y >= height) return true;
        return grid[y][x].getType() == Cell::WALL; 
    }
    bool isSlowing(int x, int y) const { 
        if(x < 0 || x >= width || y < 0 || y >= height) return false;
        return grid[y][x].getType() == Cell::SLOWING; 
    }

    const std::vector<std::string>& getUILog() const { 
        return message_log; 
    }
    const std::vector<Enemy*>& getEnemies() const {
         return enemies; 
        }
    const std::vector<EnemyBuilding*>& getBuildings() const {
         return buildings; 
        }
    const std::vector<EnemyTower*>& getTowers() const {
         return towers; 
        }

    void removeEnemy(Enemy* enemy);
    void removeBuilding(EnemyBuilding* building);
    void removeTower(EnemyTower* tower);
    void removeAlly(Ally* ally);
    void removeTrap(Trap* trap);

    void save(std::ostream& out) const;
    void load(std::istream& in);
};

#endif