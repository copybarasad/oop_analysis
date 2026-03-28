#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
private:
    int damage;
public:
    Enemy(int startHealth = 50, int dmg = 15)
        : Entity(startHealth, startHealth), damage(dmg) {}
    int getDamage() const { return damage; }
    void setDamage(int dmg) { damage = dmg; }
    
    std::string getEntityName() const override { return "Враг"; }
};

#endif
