#pragma once
#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "Position.h"

class GameField;

class EnemyTower {
private:
    Position position;
    int attackRange;
    int cooldown;
    int currentCooldown;
    bool canAttack;
    int damage;

public:
    EnemyTower(int x, int y, int attackRange = 4);

    void update(GameField* field);
    Position getPosition() const;
    bool isInRange(const Position& target) const;
    int getDamage() const { return damage; }

private:
    void attack(GameField* field, const Position& target);
};

#endif