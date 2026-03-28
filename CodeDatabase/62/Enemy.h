#pragma once
#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(std::string name, float speed, int health, int damage) : Entity(name, speed, health, damage) {}
    ~Enemy() = default;
    void makeTurn(
        std::vector<std::pair<int, int>> pathToPlayer, 
        std::pair<int, int> playerPos,
        std::function<void(int x, int y)> makeMove,
        std::function<void(int damage, int x, int y)> applyDamage
        );
};