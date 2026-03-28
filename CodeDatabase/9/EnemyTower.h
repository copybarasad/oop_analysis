#ifndef ENEMYTOWER_H
#define ENEMYTOWER_H

#include <utility>
#include "Player.h"

class EnemyTower{
public:
    EnemyTower(std::pair<int,int> pos, int range, int damage, int cooldown, int hp);

    std::pair<int,int> position() const;

    void onTurn(Player& player);

    int hp() const;
    bool isAlive() const;
    void takeDamage(int dmg);
    int damage() const;

    int cooldown() const;

    void setHp(int hp);
    void setCooldown(int value);

private:
    std::pair<int,int> pos_;
    int range_;
    int damage_;
    int cooldown_;
    int cooldownLeft_;
    int hp_;

    int Chebyshev(std::pair<int,int> a, std::pair<int,int> b) const;
};

#endif