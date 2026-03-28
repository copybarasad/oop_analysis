#ifndef RPG_ENEMY_H
#define RPG_ENEMY_H

#include "Entity.h"

class Enemy : public Entity {
public:
    Enemy(unsigned int h, unsigned int d, unsigned int viewDiameter);

    explicit Enemy(json& data);

    [[nodiscard]] std::unique_ptr<Entity> clone() const override;
};


#endif