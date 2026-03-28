#ifndef ENEMY_H
#define ENEMY_H

#include <iostream>
#include "../Entity/Entity.hpp"

class Enemy : public Entity {
    public:
    Enemy (size_t id, unsigned int maxHealthValue, unsigned int currentHealthValue, unsigned int damageValue);
    Enemy (Enemy& other, size_t id);

    unsigned int calculateDamage () const override;
};

#endif