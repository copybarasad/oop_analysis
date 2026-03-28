#ifndef ENEMY_H
#define ENEMY_H

#include "Entity.h"
#include "Serialization.h"
#include <iostream>

class Enemy : public Entity {
public:
    Enemy(int health, int baseDamage, int x, int y);

    void log() const override;

    json serialize() const;
    void deserialize(const json& j);
};

#endif // ENEMY_H