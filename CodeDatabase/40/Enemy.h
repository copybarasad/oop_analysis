#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include <string>

class Enemy : public Character{
public:
Enemy(int enemyID, const std::string& enemyName, int initialHP,
    CombatStyle style, int startX, int startY);

    void move(int deltaX, int deltaY) override;
    void attack(Character& target) override;
    bool canMoveTo(int targetX, int targetY) const override;

    void save(std::ostream& out) const override;
    void load(std::istream& in) override;
};

#endif