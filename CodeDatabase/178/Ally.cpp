#include "Ally.h"
#include "Enemy.h"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <memory>

Ally::Ally(const Position &pos, int health, int dmg) : Character(pos, health, dmg) {}

void Ally::move(const Position &newPosition)
{
    position = newPosition;
}

void Ally::act(std::vector<Enemy> &enemies)
{
    if (enemies.empty() || !isAlive())
        return;
    Enemy *nearest = nullptr;
    int minDist = 9999;
    for (auto &enemy : enemies)
    {
        if (!enemy.isAlive())
            continue;
        int dx = enemy.getPosition().x - position.x;
        int dy = enemy.getPosition().y - position.y;
        int dist = std::abs(dx) + std::abs(dy);
        if (dist < minDist)
        {
            minDist = dist;
            nearest = &enemy;
        }
    }
    if (!nearest)
        return;
    if (minDist <= 1)
    {
        nearest->takeDamage(damage);
        std::cout << "Ally attacks enemy at (" << nearest->getPosition().x << "," << nearest->getPosition().y
                  << ") for " << damage << " damage!\n";
    }
    else
    {
        Position newPos = position;
        if (nearest->getPosition().x > position.x)
            newPos.x++;
        else if (nearest->getPosition().x < position.x)
            newPos.x--;

        if (nearest->getPosition().y > position.y)
            newPos.y++;
        else if (nearest->getPosition().y < position.y)
            newPos.y--;

        position = newPos;
    }
}