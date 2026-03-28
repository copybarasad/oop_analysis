#ifndef ENEMY_TOWER_H
#define ENEMY_TOWER_H

#include "game_entity.h"
#include <iostream>

class Player;

class EnemyTower {
private:
    Position position_;
    int damage_;
    int range_;
    int cooldown_;
    int currentCooldown_;

public:
    EnemyTower(int x = 0, int y = 0, int damage = 12, int range = 2, int cooldown = 2);
    Position getPosition() const;
    int getRange() const;
    bool canAttackThisTurn() const;
    void attackPlayer(Player& player) const;
    void tick();
    void resetCooldown();

    void save(std::ostream& os) const;
    void load(std::istream& is);
};

#endif