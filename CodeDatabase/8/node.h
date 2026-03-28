#pragma once
#include "type.h"

class Player;
class Enemy;

class Node {
    Type type;
    int trapDamage; 
public:
    Node();
    bool hasEnemy() const;
    bool hasPlayer() const;
    bool hasObstacle() const;
    bool isEmpty() const;
    bool isPassable() const;
    Type getType() const;
    void setObstacle();
    void setPlayer();
    void setEnemy();
    void setEmpty();
    void removeEntity();
    void removeObstacle();
    
    bool hasTrap() const;
    void setTrap(int damage);
    void removeTrap();
    int triggerTrap();
    
    bool hasTower() const;
    void setTower();
    void removeTower();
};