#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Enemy : public Character {
public:
    Enemy(int initialHealth, int enemyDamage);
    
    Enemy(const Enemy& other) = default;
    Enemy(Enemy&& other) = default;
    Enemy& operator=(const Enemy& other) = default;
    Enemy& operator=(Enemy&& other) = default;
    ~Enemy() override = default;
    
    Character* clone() const override;
    void performAction() override;
};

#endif