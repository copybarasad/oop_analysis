#pragma once
#include "Cell.h"
#include "Entity.h"
#include <memory>
#include <vector>
#include <iostream>

class Player;
class Enemy;

class Field {
private:
    int width, height;
    Cell** grid;
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Enemy>> enemies;

    void initializeWalls();

public:
    Field(int w, int h);
    
    Field(const Field& other);
    Field& operator=(const Field& other);
    
    Field(Field&& other) noexcept;
    Field& operator=(Field&& other) noexcept;
    
    ~Field();

    void setPlayer(std::unique_ptr<Player> p);
    void addEnemy(std::unique_ptr<Enemy> enemy);
    void removeEnemy(Enemy* enemy);
    
    bool isValidPosition(int x, int y) const;
    bool isWall(int x, int y) const;
    bool isEmpty(int x, int y) const;
    bool hasPlayerAt(int x, int y) const;
    bool hasEnemyAt(int x, int y) const;
    
    Player* getPlayer() const;
    Enemy* getEnemyAt(int x, int y) const;
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const;
    
    void clearCell(int x, int y);
    void setWall(int x, int y);
    void placeEntity(Entity* entity);
    
    int getWidth() const;
    int getHeight() const;
    
    void draw() const;
};