#pragma once
#include "type.h"

class Player;
class Enemy;

class Cell {
    Type type;
public:
    Cell();
    bool hasEnemy() const;
    bool hasPlayer() const;
    bool isEmpty() const;
    bool isTrap() const;
    bool isDamageTrap() const;
    bool isPassable() const;
    Type getType() const;
    void setTrap();
    void setDamageTrap();
    void setObstacle();
    void setPlayer();
    void setEnemy();
    void setEmpty();
    void setTower();
    void setDamageTower();
    void removeEntity();
};
