#ifndef FIELD_H
#define FIELD_H

#include <vector>
#include "PositionField.h"
#include "Tower.h" 

class Player;
class Enemy;

class Field {
private:
    int width, height;
    std::vector<std::vector<Position_field>> pos;
    std::vector<Tower> towers;
    std::vector<std::pair<int, int>> walls;
    
    int trapX, trapY;
    int trapDamage;
    bool hasActiveTrap;

    void Field_initialize();

public:

    Field(int w, int h, bool generatewalls = true);
    Field(const Field& other);
    Field(Field&& other) noexcept;
    Field& operator=(const Field& other);
    Field& operator=(Field&& other) noexcept;

    void generate_block(int level, int playerX, int playerY, 
    const std::vector<Enemy>& enemies, 
    const std::vector<Tower>& towers);
    void display() const;
    bool valid_pos(int x, int y) const;
    bool CanMove(int x, int y) const;
    void updateField(int playerX, int playerY, const std::vector<std::pair<int, int>>& enemyPositions); 

    void setTrap(int x, int y, int damage);
    void checkTrap(int x, int y, std::vector<Enemy>& enemies);
    void removeTrap();
    bool isTrapActive() const;

    void addTower(int x, int y, int range = 4, int health = 80);
    void updateTowers(Player& player, std::vector<Enemy>& enemies);
    std::vector<Tower>& getTowers();
    const std::vector<Tower>& getTowers() const;

    std::vector<std::pair<int, int>> getAllWalls() const;
    void setWall(int x, int y);

    int getTrapX() const;
    int getTrapY() const;
    int getTrapDamage() const;
    int getWidth() const;
    int getHeight() const;
};

#endif