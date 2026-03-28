#ifndef BASEENEMY_H
#define BASEENEMY_H

#include "GameObject.h"
#include <cmath>

// Предварительные объявления
class GameField;
class Player;

class BaseEnemy : public GameObject {
protected:
    int m_damage;

public:
    BaseEnemy(int health, int damage, int x, int y)
        : GameObject(health, health, x, y), m_damage(damage) {}

    virtual ~BaseEnemy() = default;

    int getDamage() const { return m_damage; }

    bool canAttackPlayer(const Player& player) const;

    virtual void makeMove(GameField& field, Player& player) = 0;
};

#endif // BASEENEMY_H