#ifndef ENEMY_H
#define ENEMY_H

#include "entity.h"

class Enemy : public Entity {
public:
    Enemy(int health, int x, int y, int damage);
    Enemy(int health, int x, int y);
    
    void save(std::ostream& os) const override;
    void load(std::istream& is) override;
};

#endif