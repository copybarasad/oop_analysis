#pragma once

#include "Position.h"

class EnemyBuilding {
private:
    Position position;
    int spawn_Counter;
    int spawn_Range;
    int health;

public:
    EnemyBuilding(int x, int y, int spawn_Range = 5, int health = 50);

    bool should_SpawnEnemy() const;
    void update();
    void reset_Counter();

    void take_Damage(int damage);
    bool isAlive() const;
    int get_Health() const;
    int get_SpawnCounter() const;
    int get_SpawnRange() const;

    const Position& get_Position() const;
    
    void setHealth(int h);
    void setSpawnCounter(int counter);
};
