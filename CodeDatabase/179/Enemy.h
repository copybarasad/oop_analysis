#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"

class Enemy : public GameObject {
private:
    int damage;

public:
    Enemy(int health = 30, int damage = 10, const std::string& name = "Enemy");
    
    int getDamage() const;
    
    std::string getType() const override;
};

#endif
