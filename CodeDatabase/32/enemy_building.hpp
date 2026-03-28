#ifndef ENEMY_BUILDING_H
#define ENEMY_BUILDING_H

#include "enemy.hpp"
#include "field.hpp"

#include <vector>
#include <cstdlib>
#include <iostream>

class EnemyBuilding{
private:
    int spawn_delay_;
    int counter_;
    int x_;
    int y_;
    int health_;
    bool destroyed_;
public:
    EnemyBuilding(int x, int y, int spawn_delay_);
    void Tick(std::vector<Enemy>& enemies, Field& field);
    int GetX() const; 
    int GetY() const;

    void TakeDamage(int damage, Field& field);
    bool IsDestroyed() const;
    int GetHealth() const;

    void SetDestroyed(bool value) { destroyed_ = value; }
};
#endif