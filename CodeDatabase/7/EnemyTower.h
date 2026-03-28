#pragma once
#include <string>

class Player;

class EnemyTower {
private:
    int x, y;
    int range;
    int damage;
    int cooldown;
    int currentCooldown;

public:
    EnemyTower(int posX, int posY, int range, int damage, int cooldown);
    void update(Player& player);
    int getX() const;
    int getY() const;
};
