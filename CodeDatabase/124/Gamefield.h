#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "cell.h"
#include "trap.h"
#include "enemytower.h"
#include "mapmanager.h"
#include "objectplacer.h"  
#include <vector>
#include <stdexcept>
#include <memory>   

class GameField {
private:
    std::unique_ptr<MapManager> map;         
    std::unique_ptr<ObjectPlacer> objectPlacer;   
    std::vector<Trap> traps; 
    std::vector<EnemyTower> towers;

public:
    GameField(int w = 15, int h = 15); 
    ~GameField() = default; 

    GameField(const GameField&) = delete;
    GameField& operator=(const GameField&) = delete;
    GameField(GameField&&) = default;
    GameField& operator=(GameField&&) = default;
    
    void reset(int w, int h);
     
    int getWidth() const { return map ? map->getWidth() : 0; }
    int getHeight() const { return map ? map->getHeight() : 0; }
    Cell& getCell(int x, int y) { 
        if (!map) throw std::runtime_error("Карта не инициализирована");
        return map->getCell(x, y); 
    }
    const Cell& getCell(int x, int y) const { 
        if (!map) throw std::runtime_error("Карта не инициализирована");
        return map->getCell(x, y); 
    }
    bool isValidPosition(int x, int y) const { 
        return map && map->isValidPosition(x, y); 
    }
    void addObstacles(int count) { 
        if (map) map->addObstacles(count); 
    }
    void addObstacle(int x, int y) { 
        if (map) map->addObstacle(x, y); 
    }
    bool isCellEmpty(int x, int y) const { 
        return map && map->isCellEmpty(x, y); 
    }
    void clearCell(int x, int y) { 
        if (map) map->clearCell(x, y); 
    }
     
    bool placePlayer(int x, int y) { 
        return objectPlacer && objectPlacer->placePlayer(x, y); 
    }
    bool placeEnemy(int x, int y) { 
        return objectPlacer && objectPlacer->placeEnemy(x, y); 
    }
    bool moveObject(int fromX, int fromY, int toX, int toY) { 
        return objectPlacer && objectPlacer->moveObject(fromX, fromY, toX, toY); 
    }
    bool findPlayerPosition(int& x, int& y) const { 
        return objectPlacer && objectPlacer->findPlayerPosition(x, y); 
    }
    bool findEnemyPosition(int& x, int& y) const { 
        return objectPlacer && objectPlacer->findEnemyPosition(x, y); 
    }
     
    bool placeTrap(int x, int y, int damage = 20);
    void updateTraps(Enemy& enemy);
    bool placeTower(int x, int y);
    void updateTowers(Player& player);
    std::vector<EnemyTower>& getTowers();
    const std::vector<EnemyTower>& getTowers() const;
    bool hasTowerAt(int x, int y) const;
    bool hasTrapAt(int x, int y) const;
    const std::vector<Trap>& getTraps() const { return traps; }
    
private:
    void displayTowersInfo() const;
};

#endif